#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
int main(int argc, char *argv[]){
    // char *argv[]:指针数组
    printf("%s/n",getenv("PATH"));
    return 0;
}