#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
int main(){
    // while(1){
    //     printf("I am a process, pid: %d \t ppid: %d!\n",getpid(),getppid());
    //     sleep(1);
    // }

    pid_t id = fork();

    if(id == 0){
        //子进程
        int cnt = 10;
        while(cnt){
            printf("我是子进程，pid:%d\n",getpid());
            cnt--;
            sleep(1);
        }
        printf("我是子进程,我进入Z状态了，pid:%d\n",getpid());
        exit(0);
        
    }
    else if(id > 0){
        //父进程
        // printf("我是父进程，pid:%d \t ppid:%d\n",getpid(),getppid());
        while(1){
            sleep(1);
        }
    }
    // printf("我是进程，pid:%d \n",getpid());

    sleep(1);

    return 0;
}