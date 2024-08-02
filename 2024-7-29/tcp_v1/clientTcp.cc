#include"include.h"

int main(int argc, char *argv[]){
    if(argc != 3){
        std::cerr << "Usage:\n\t" << argv[0] << " serverIp serverPort" << std::endl;  
    }
    string serverIp = argv[1];
    uint16_t serverPort = atoi(argv[2]);

    // 1.创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        cerr << "客户端：创建套接字失败" << endl;
        exit(1);
    }
    // 2.发送连接请求
    // 2.1 填充远端主机的基本信息
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverPort);
    inet_aton(serverIp.c_str(),&server.sin_addr);
    // 2.2 发起请求
    if(connect(sock, (const struct sockaddr *)&server, sizeof(server)) != 0){
        cerr << "客户端：connect 失败" << endl;
        exit(2);
    }
    cout << "客户端：connect 成功" << endl;

    bool quit = false;
    while(!quit){
        char buffer[BUFFER_SIZE];
        cout << "请输入你的消息>>> ";
        fflush(stdout);
        int s = read(0,buffer,sizeof(buffer) - 1);
        
        if(s != -1){
            buffer[s-1] = '\0'; //读到的字符中有\n
            // 将信息发给server
            if(strcasecmp(buffer,"quit") == 0){
                quit = true;
            }
            s = write(sock,buffer,sizeof(buffer));
            if(s <= 0){
                break;
            }else{
                s = read(sock, buffer, sizeof(buffer));
                if(s > 0){
                    buffer[s] = '\0';
                    cout << "Server Echo>>> " << buffer << endl;
                }
            }
        }else{
            cerr << "读取错误" << endl;
            continue;
        }

    }
    close(sock);
    return 0;
}