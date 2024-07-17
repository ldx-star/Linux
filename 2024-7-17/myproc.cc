#include<iostream>
#include<unistd.h>
#include <signal.h>


using namespace std;

// typedef void(*handler)();
int cnt = 0;

static void showSigset(sigset_t *sig_set){
    for(int i = 1; i <= 31;i ++){
        if(sigismember(sig_set,i)){
            cout << "1";
        }else{
            cout << "0";
        }
    }
    cout << endl;
}

void handler(int signo){
    printf("进程[%d]收到信号： %d\n",getpid(), signo); 
    while(true){
        cout << "信号处理中" << endl;
        sigset_t pending;
        sigpending(&pending);
        showSigset(&pending);
        sleep(1);

    }       
}



int main(){
    struct sigaction act,oact;
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);  
    for(int i = 1; i <= 31; i++){
        sigaddset(&act.sa_mask, i);
    }
    sigaction(2,&act,&oact);
    while(1){}


    // //屏蔽2号信号
    // sigset_t bsig,obsig;
    // sigemptyset(&bsig);
    // sigemptyset(&obsig);
    
    // sigaddset(&bsig,2);
    // //修改信号屏蔽字
    // sigprocmask(SIG_SETMASK,&bsig,&obsig);


    // signal(2,handler);
    // sigset_t pendings;
    // while(true){
    //     sigemptyset(&pendings);
    //     if(sigpending(&pendings) == 0){
    //         showSigset(&pendings);
    //         sleep(1);
    //     }
    // }
    return 0;
}