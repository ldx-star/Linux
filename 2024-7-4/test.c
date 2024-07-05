#include <stdio.h>

#define M 10

int main(){
    printf("M = %d\n", M);
    // printf("M = %d", M);
    // printf("M = %d", M);
    printf("Hello World!\n");

#ifdef DEBUG
    printf("debug\n");
#else
    printf("release\n");
#endif
    return 0;
}