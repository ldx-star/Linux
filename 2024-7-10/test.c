#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main(){
    // umask(0);
    // int fda = open("log1.txt",O_RDONLY | O_CREAT | O_TRUNC, 0666);
    // int fdb = open("log2.txt",O_RDONLY | O_CREAT | O_TRUNC, 0666);
    // int fdc = open("log3.txt",O_RDONLY | O_CREAT | O_TRUNC, 0666);
    // int fdd = open("log4.txt",O_RDONLY | O_CREAT | O_TRUNC, 0666);

    // printf("fda:%d\n",fda);
    // printf("fdb:%d\n",fdb);
    // printf("fdc:%d\n",fdc);
    // printf("fdd:%d\n",fdd);

    // close(fda);
    // close(fdb);
    // close(fdc);
    // close(fdd);
    
    // printf(" hello printf "); // 封装write

    // const char *buf = " hello write ";
    // write(1,buf, strlen(buf));    

    const char *str1 = "hello printf\n";
    const char *str2 = "hello fprintf\n";
    const char *str3 = "hello fputs\n";
    const char *str4 = "hello write\n";

    printf(str1);
    fprintf(stdout,str2);
    fputs(str3,stdout);

    write(1,str4, strlen(str4));

    fork();

    return 0;
}