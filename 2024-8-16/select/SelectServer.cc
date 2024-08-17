#include "Sock.hpp"
#include <sys/select.h>


#define DFL -1

int fdsArray[sizeof(fd_set) * 8] = {0}; //保存历史合法fd
int gnum = sizeof(fdsArray) / sizeof(fdsArray[0]);

static void showArray(int arr[], int num)
{
    cout << "当前合法sock list# ";
    for (int i = 0; i < num; i++)
    {
        if (arr[i] == DFL)
            continue;
        else
            cout << arr[i] << " ";
    }
    cout << endl;
}

static void HandlerEvent(int listenSock, fd_set &readfds){
    for(int i = 0; i < gnum; i++){
        if(fdsArray[i] == DFL) continue;
        if(i == 0 && fdsArray[i] == listenSock){
            // 监听文件描述符准备就绪
            if(FD_ISSET(listenSock, &readfds)){
                cout << "已经有一个新的链接到来" << endl;
                string clientIp;
                uint16_t clientPort = 0;
                int sock = Sock::Accept(listenSock, &clientIp, &clientPort);
                if(sock < 0){
                    return;
                }
                cout << "获取新连接成功: " << clientIp << ":" << clientPort << " | sock: " << sock << endl;
                int k = 0;
                while(k < gnum){
                    if(fdsArray[k] == DFL){
                        // 找到文件描述符集合中空闲的位置
                        break;
                    }
                    k++;
                }
                if(k == gnum){
                    cerr << "我的服务器已经到了最大的上限了，无法在承载更多同时保持的连接了" << endl;
                    close(sock);
                }else{
                    fdsArray[k] = sock; //将用于通信的sock加入到待监听的文件描述符集中
                    showArray(fdsArray, gnum);
                }
            }
        }else{
            // 其他IO事件准备就绪
            if(FD_ISSET(fdsArray[i], &readfds)){
                char buffer[1024];
                ssize_t s = recv(fdsArray[i],buffer,sizeof(buffer) - 1, 0);// 非阻塞
                if(s > 0){
                    buffer[s] = 0;
                    cout << "client[" << fdsArray[i] << "]# " << buffer << endl; 
                }else if(s == 0){
                    cout << "client[" << fdsArray[i] << "] quit, server close " << fdsArray[i] << endl;
                    close(fdsArray[i]);
                    fdsArray[i] = DFL; // 去除对该文件描述符的select事件监听
                    showArray(fdsArray, gnum);
                }else{
                    cout << "client[" << fdsArray[i] << "] error, server close " << fdsArray[i] << endl;
                    close(fdsArray[i]);
                    fdsArray[i] = DFL; // 去除对该文件描述符的select事件监听
                    showArray(fdsArray, gnum);
                }
            }
        }
    }
}

int main(int argc, char *argv[]){
    if(argc != 2){
        cerr << "Usage:" << argv[0] << " " << "8080" << endl;
    }
    int listenSock = Sock::Socket();
    Sock::Bind(listenSock,atoi(argv[1]));
    Sock::Listen(listenSock);

    for(int i = 0; i < gnum; i++){
        fdsArray[i] = DFL;
    }
    fdsArray[0] = listenSock;
    while (true){
        int maxFd = DFL;
        fd_set readfds;
        FD_ZERO(&readfds); //将文件描述符集设为0
        for(int i = 0; i < gnum; i++){
            if(fdsArray[i] == DFL) continue; // 过滤不合法的fd
            FD_SET(fdsArray[i], &readfds);  // 将所有的合法的fd添加到readfds中，方便select统一进行监听
            if(maxFd < fdsArray[i]){
                maxFd = fdsArray[i];
            }
        }
        struct timeval timeout = {100,0};
        // accept获取连接，本质上是把客户端建立的链接提到前端来，客户端与服务端需要先三次握手建立链接，也是IO.如果没有客户端接入就会阻塞等待
        // 使用select编写多路转接代码，确保链接已建立在使用accept
        int n = select(maxFd + 1,&readfds, nullptr, nullptr, &timeout);
        switch (n)
        {
        case 0:
            cout << "time out ..." << (unsigned int)time(nullptr) << endl;
            break;
        case -1:
            cerr << errno << ":" << strerror(errno) << endl;
        default:
            HandlerEvent(listenSock,readfds);
            break;
        }

    }



    return 0;
}