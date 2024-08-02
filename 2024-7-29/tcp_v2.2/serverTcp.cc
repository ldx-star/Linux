#include "include.h"

// 多进程
class ServerTcp{
public:
    ServerTcp(uint16_t port, const std::string &ip = ""):listenSock_(-1),port_(port),ip_(ip){}
    ~ServerTcp(){}
public:
    void init(){
        // 1.创建socket
        listenSock_ = socket(AF_INET,SOCK_STREAM,0);
        if(listenSock_ < 0){
            cerr << "创建监听套接字失败" << endl;
            exit(1);
        }
        cout << "创建套接字成功" << endl;

        // 2.bind
        // 2.1填充服务器信息
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(port_);
        ip_.empty() ? (local.sin_addr.s_addr = INADDR_ANY):(inet_aton(ip_.c_str(), &local.sin_addr));
        // 2.2 bind
        if(bind(listenSock_, (const struct sockaddr *)&local, sizeof local) < 0){
            cerr << "bind 失败" << endl;
            exit(2);
        }
        cout << "bind 成功" << endl;

        // 3. tcp是面向连接的，需要进行监听
        if(listen(listenSock_,5) < 0){
            cerr << "listen 失败" << endl;
            exit(3);
        }
        cout << "listen 成功" << endl;
    }

    void loop(){
        while(true){
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            // 4.获取连接, 返回的是一个新的socket_fd
            int serviceSock = accept(listenSock_,(struct sockaddr *)&peer, &len);
            if(serviceSock < 0){
                cerr << "获取连接失败" << endl;
                continue;
            }
            // 4.1 获取客户端基本信息
            uint16_t peerPort = ntohs(peer.sin_port);
            string peerIp = inet_ntoa(peer.sin_addr);

            // 5.提供服务
            // transService(serviceSock,peerIp,peerPort); // 一旦进入transService，主执行流无法向后执行, 需要等提供服务完毕后才能accept
            
            // 爷爷进程
            pid_t id = fork();
            if(id == 0){
                // 爸爸进程
                close(listenSock_);
                if(fork() > 0) exit(0);
                //孙子进程
                transService(serviceSock,peerIp,peerPort);
                exit(0);
            }
            close(serviceSock);
            waitpid(id,nullptr,0);// 由孙子进程去执行transService, 爸爸进程立即退出，因此可以进行阻塞式等待，而由于爸爸进行退出，孙子进程交由OS处理，不需要等待
        }
    }

    // 大小写转换服务
    void transService(int sock, const string &clientIp, uint16_t clientPort){
        assert(sock >= 0);
        assert(!clientIp.empty());
        assert(clientPort >= 1024);
        char inbuffer[BUFFER_SIZE];
        while(true){
            ssize_t s = read(sock, inbuffer, sizeof(inbuffer) - 1);
            if(s > 0){
                inbuffer[s] = '\0';
                // strcasecmp 无视大小写比较
                if(strcasecmp(inbuffer, "quit") == 0){
                    cout << "client quit -- " << clientIp << "[ " << clientPort << "]";
                    break;
                }
                for(int i = 0; i < s; i++){
                    if(isalpha(inbuffer[i]) && islower(inbuffer[i])){
                        inbuffer[i] = toupper(inbuffer[i]);
                    }
                }

                write(sock, inbuffer, strlen(inbuffer)); // sock文件可读可写，称为全双工
            }
            else if(s == 0){
                // pipe中 s == 0 代表对端关闭
                // s == 0 表示客户端退出
                cout << "client quit -- " << clientIp << "[ " << clientPort << "]";
                break;
            }
        }
        close(sock); // 如果一个进程对应的文件打开了，但是没有归还，称为文件描述符泄漏
        cout << "server close" << sock << "done" << endl;

    }
private:
    int listenSock_; //监听套接字
    uint16_t port_;
    string ip_;
};

int main(int argc, char* argv[]){
    if(argc != 3 && argc != 2){
        cerr << "Usage:\n\t" << argv[0] << " port ip" << std::endl;
        exit(4); 
    }
    uint16_t port = atoi(argv[1]);
    string ip;
    if(argc == 3)ip = argv[2];
    ServerTcp svr(port, ip);
    svr.init();
    svr.loop(); 
    return 0;
}