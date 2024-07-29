#include<iostream>
#include<string>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<cstring>
using namespace std;
class UdpServer{
public:
    UdpServer(int port, string ip = ""):port_(port), ip_(ip) {}
    ~UdpServer(){}
public:
    void init(){
        // 1. 创建套接字接口
        sockfd_ = socket(AF_INET,SOCK_DGRAM,0);
        if(sockfd_ < 0){
            // 创建失败
            cerr << "服务器端：创建套接字接口失败" << endl;
            exit(1);
        }
        cout << "服务器端：创建套接字接口成功"  << endl;

        // 2. 绑定网络信息， 指明 ip+port
        
        // 2.1 填充信息到sockaddr_in
        struct sockaddr_in local;
        memset(&local,0, sizeof(local));
        // 填充协议家族
        local.sin_family = AF_INET;
        // 填充端口号,由于计算器大小端编码方式可能会不一样，在进行网络传输的时候需要将本地字节序转为网络字节序
        local.sin_port = htons(port_);
        // 填充ip地址
        // INADDR_ANY: 程序员不关心会bind到哪一个ip, 任意地址bind， 所有服务器一般的做法。也就是说，所有ip的客户端都能连接我们的服务器
        // inet_addr: 填充指定ip， 将字符串类型的ip地址转成uint32_t类型，并且自动h->n
        local.sin_addr.s_addr = ip_.empty() ? htonl(INADDR_ANY) : inet_addr(ip_.c_str());

        // 2.2 bind 网络信息
        if(bind(sockfd_,(const struct sockaddr*) &local,sizeof(local) ) == -1){
            cerr << "服务器端：绑定失败" << endl;
            exit(2);
        }
        cout << "服务器端：绑定成功" << endl;
    }

    void start(){
        // 服务器设计的时候都是死循环
        char inbuffer[1024]; // 将来读取的数据，都放到这里
        char outbuffer[1024]; // 将来发送的数据，都放这里
        while(true){
            struct sockaddr_in peer; // 输出型参数，用于带回客户端的网络信息
            socklen_t len = sizeof(peer);

            // 接收消息
            ssize_t s = recvfrom(sockfd_, inbuffer, sizeof(inbuffer)-1,0,(struct sockaddr*)&peer,&len);
            if(s > 0){
                inbuffer[s] = 0;
            }else if(s == -1){
                continue;
            }

            // 读取成功
            string peerIp = inet_ntoa(peer.sin_addr); // 获得对方的ip
            uint32_t peerPort = ntohs(peer.sin_port); // 获取对方的端口号

            // 打印客户端发来的消息
            printf("%s\n", inbuffer);
        }
    }
private:
    int port_;
    int sockfd_;
    string ip_;
};
int main(int argc, char *argv[]){
    if(argc != 2 && argc != 3){
        std::cout << "Usage:\n\t" << argv[0] << " port [ip]" << std::endl;
    }
    uint16_t port = atoi(argv[1]);
    std::string ip;
    if(argc == 3){
        ip = argv[2];
    }
    UdpServer svr(port,ip);
    svr.init();
    svr.start();
    
    return 0;
}