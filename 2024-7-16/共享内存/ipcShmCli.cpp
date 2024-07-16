// 写
#include "comm.hpp"
#include "Log.hpp"
using namespace  std;

int main(){
    key_t key = CreateKey();
    Log() << "key: " << key << endl;
    int shmId = shmget(key,MEM_SIZE, IPC_CREAT);
    if(shmId < 0){
        //创建失败
        Log() << "shmget: " << strerror(errno) << endl;
        return 2;
    }
    Log() << "create shm success, shmId: " << shmId << endl;
    
    //关联
    char *str = (char*)shmat(shmId,nullptr,0);

    //使用
    int cnt = 0;
    while(cnt <= 26){
        str[cnt] = 'A' + cnt;
        ++cnt;
        str[cnt] = '\0';
        sleep(1);
    }   

    // 取消关联
    shmdt(str); 


    return 0;
}