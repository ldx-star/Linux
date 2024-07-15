// 读
#include "comm.h"

using namespace std;

int main(){
    // 创建管道文件
    umask(0);
    if(mkfifo(IPC_PATH,0600) != 0){
        cerr << "mkfifo error" << endl;
        return 1;
    }

    // 打开文件
    int pipeFd = open(IPC_PATH,O_RDONLY);
    if(pipeFd < 0){
        cerr << "open fifo error" << endl;
        return 2;
    }
    char buffer[1024];
    while(true){
        ssize_t s = read(pipeFd,buffer,sizeof(buffer) - 1);
        if(s > 0){
            buffer[s-1] = '\0';
            cout << "客户端->服务器端# " << buffer << endl ; 
            // cout << "客户端->服务器端# \n";
            // printf("dsad\n");
        }else if(s == 0){
            cout << "客户端退出" << endl;
            break;
        }else{
            // do nothing
        }
    }
    close(pipeFd);
    cout << "hello server" << endl;
    return 0;
}