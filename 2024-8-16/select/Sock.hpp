#pragma once
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;


class Sock
{
public:
    static const int gbacklog = 20;
    
    static int Socket(){
        int listenSock = socket(PF_INET, SOCK_STREAM, 0);
        if(listenSock < 0){
            exit(1);
        }
        //设置套接字接口，使得服务端终端后，可以立即bind相同的端口
        int opt = 1;
        setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
        return listenSock;
    }

    static void Bind(int socket, uint16_t port){
        struct sockaddr_in local;
        memset(&local, 0, sizeof local);
        local.sin_family = PF_INET;
        local.sin_port = htons(port);
        local.sin_addr.s_addr = INADDR_ANY;

        if(bind(socket, (const struct sockaddr *)&local, sizeof local) < 0){
            exit(2);
        }
    }

    static void Listen(int socket){
        if(listen(socket, gbacklog) < 0){
            exit(3);
        }
    }

    static int Accept(int socket, std::string *clientip, uint16_t *clientport){
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);

        int serviceSock = accept(socket, (struct sockaddr *)&peer, &len);
        if(serviceSock < 0){
            //获取链接失败
            return -1;
        }
        if(clientport) *clientport = ntohs(peer.sin_port);
        if(clientip) *clientip = inet_ntoa(peer.sin_addr);
        return serviceSock;
    }
};