// 写
#include "comm.h"
using namespace  std;

int main(){
    int pipeFd = open(IPC_PATH, O_WRONLY);
    char msg[1024];
    while(true){
        ssize_t s = read(0,msg,sizeof(msg) - 1);
        if(s > 0){
            msg[s] = '\0';
        }
        // msg[strlen(msg) - 1] = '\0';
        write(pipeFd,msg,strlen(msg));
    }
    close(pipeFd);

    cout << "hello client" << endl;
    return 0;
}