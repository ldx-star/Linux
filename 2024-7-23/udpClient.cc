#include<iostream>
#include<string>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<cstring>
#include<cassert>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]){
    if(argc != 3){
          cout << "Usage:\n\t" << argv[0] << " server_ip server_port" << endl;
        exit(1);
    }
    // 1. 设置访问服务器ip以及端口号
    string server_ip = argv[1];
    uint16_t server_port = atoi(argv[2]);

    // 2.创建客户端
    // 2.1 创建socket
    int sockfd = socket(AF_INET, SOCK_DGRAM,0);
    assert(socket > 0);

    // 客户端也需要bind，但是对于客户端来说，不需要有固定的ip和port，因此这部分交给os来做
    // 对于服务端来说，它是给所有客户端提供服务，需要要有固定的ip和port的，所以服务器端需要bind

    // 2.2 创建server地址信息
    struct sockaddr_in server;
    memset(&server,0,sizeof(server));
    server.sin_port = htons(server_port);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_ip.c_str());

    string buf;
    while(true){
        cout << "Please Enter#";
        getline(std::cin, buf);
        //个server发消息
        sendto(sockfd,buf.c_str(),buf.size(),0,(const struct sockaddr *)&server, sizeof(server));
    }
    close(sockfd);


    return 0;
}