#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <signal.h>
#include <cstring>
#include <assert.h>
#include <sys/wait.h>
#include <fstream>
using namespace std;

#define CRLF "\r\n"
#define SPACE " "
#define SPACE_LEN strlen(SPACE)
#define HOME_PAGE "index.html"
#define ROOT_PATH "wwwroot"

string getPath(std::string http_request)
{
    std::size_t pos = http_request.find(CRLF);
    if(pos == std::string::npos) return "";
    std::string request_line = http_request.substr(0, pos);
    //GET /a/b/c http/1.1
    std::size_t first = request_line.find(SPACE);
    if(pos == std::string::npos) return "";
    std::size_t second = request_line.rfind(SPACE);
    if(pos == std::string::npos) return "";

    std::string path = request_line.substr(first+SPACE_LEN, second - (first+SPACE_LEN));
    if(path.size() == 1 && path[0] == '/') path += HOME_PAGE;
    return path;
}

string readFile(const std::string &recource)
{
    std::ifstream in(recource, std::ifstream::binary);
    if(!in.is_open()) return "404";
    std::string content;
    std::string line;
    while(getline(in, line)) content += line;
    in.close();
    return content;
}

void handlerHttpRequest(int sock){
    char buffer[10240];
   
    ssize_t s = read(sock, buffer, sizeof buffer);// 接收请求信息
    if(s > 0)cout << buffer; 


    // // 重定向
    // string response = "HTTP/1.1 302 Temporarily Moved\r\n";
    // response += "Location: https://www.baidu.com\r\n";
    // response += "\r\n";

    // send(sock,response.c_str(),response.size(),0);


    //从请求中获取路径
    string path = getPath(buffer);
    cout << ROOT_PATH + path << endl;
    string html = readFile(ROOT_PATH + path);// 从文件中得到正文
    //开始响应
    string response;
    response = "HTTP/1.0 200 OK\r\n";
    response += "Content-Type: text/html\r\n";
    response += "Content-Length: " + to_string(html.size()) + "\r\n";
    response += "Set-Cookie: this is my cookie content\r\n";
    response += "\r\n";

    // 响应正文
    response += html;

    send(sock,response.c_str(),response.size(),0);
}

class ServerTcp
{
public:
    ServerTcp(uint16_t port, const string &ip = "")
        :port_(port),ip_(ip),listenSock_(-1),quit_(false)
    {}
    ~ServerTcp(){
        if(listenSock_ > 0){
            close(listenSock_);
        }
    }
public:
    void init(){
        // 创建socket
        listenSock_ = socket(AF_INET, SOCK_STREAM,0);
        if(listenSock_ < 0){
            exit(1);
        }
        
        // bind
        struct sockaddr_in local;
        memset(&local, 0, sizeof local);
        local.sin_family = AF_INET;
        local.sin_port = htons(port_);
        ip_.empty() ? (local.sin_addr.s_addr = INADDR_ANY) : (inet_aton(ip_.c_str(), &local.sin_addr));
        if(bind(listenSock_, (const struct sockaddr *) &local, sizeof local) < 0){
            exit(2);
        }
        if(listen(listenSock_,5) < 0){
            exit(3);
        }
    }

    void loop(){
        signal(SIGCHLD,SIG_IGN);
        while(!quit_){
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);

            int serviceSock = accept(listenSock_, (struct sockaddr *)&peer, &len);
            if(quit_){
                break;
            }
            pid_t id = fork();
            assert(id != -1);
            if(id == 0){
                close(listenSock_);
                if(fork() > 0) exit(0);
                handlerHttpRequest(serviceSock);
                exit(0);
            }
            close(serviceSock);
            wait(nullptr);
        }

    }
private:
    // port
    uint16_t port_;
    // ip
    std::string ip_;
    // 安全退出
    bool quit_;
    // sock
    int listenSock_;
};