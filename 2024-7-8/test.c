#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>

int globel = 100;

int main(int argc, char *argv[]){
    // pid_t id = fork();
    // if(id == 0){
    //     // 子进程
    //     int cnt = 0;
    //     while(1){
    //         printf("我是子进程，pid:%d  ppid:%d  globel:%d  globel_addr:%p \n\n", getpid(),getppid(),globel,&globel);
    //         if(cnt == 5){
    //             printf("我是子进程，我修改了globel！！！\n\n");
    //             globel = 200;
    //         }
    //         cnt++;
    //         sleep(1);
    //     }
    // }else if(id > 0){
    //     // 父进程        
    //     while(1){
    //         printf("我是父进程，pid:%d  ppid:%d  globel:%d  globel_addr:%p \n\n", getpid(),getppid(),globel,&globel);
    //         sleep(1);
    //     }

    // }
    // int cnt = 0;
    // while(1){
    //     pid_t id = fork();

    //     if(id == 0){
    //         // 子进程
    //         cnt++;
    //         printf("我是子进程，pid:%d\n", getpid());
    //         sleep(20);
    //         exit(0);
    //     }else if(id < 0){
    //         printf("创建进程失败\n");
    //         break;
    //     }
    // }
    // printf("一共创建%d个子进程\n",cnt);

    return 0;
}