// 读
#include "comm.hpp"
#include "Log.hpp"

using namespace std;



int main(){
    key_t key = CreateKey();
    Log() << "key: " << key << endl;
    int shmId = shmget(key,MEM_SIZE, FLAG | 0666); // 0666 权限
    if(shmId < 0){
        //创建失败
        Log() << "shmget: " << strerror(errno) << endl;
        return 2;
    }
    Log() << "create shm success, shmId: " << shmId << endl;
    
    //关联
    char *str = (char*)shmat(shmId,nullptr,0);

    //使用
    while(true){
        cout << str << endl;
        sleep(1);
    }

    // 取消关联
    shmdt(str); 


    // 删除
    shmctl(shmId,IPC_RMID,nullptr);

    return 0;
}