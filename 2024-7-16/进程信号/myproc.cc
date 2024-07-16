#include<iostream>
#include<unistd.h>
#include <signal.h>


using namespace std;

// typedef void(*handler)();
int cnt = 0;

void handler(int signo){
    printf("hello: %d\n", cnt);        
    exit(1);
}

int main(){
    alarm(1);
    signal(14,handler);    
    while(1){
        cnt++;
        // printf("hello: %d\n", cnt);        
    }
    return 0;
}