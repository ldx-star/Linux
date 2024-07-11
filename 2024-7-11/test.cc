#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
int main(){
    //stdout
    printf("hello printf 1\n");
    fprintf(stdout, "hello fprintf 1\n");
    fputs("hello fputs 1\n", stdout);

    //stderr
    fprintf(stderr, "hello fprintf 2\n");
    fputs("hello fputs 2\n", stderr);
    perror("hello perror 2");

    std::cout << "hello cout 1" << std::endl;
    std::cerr << "hello cerr 2" << std::endl;

    return 0;
}