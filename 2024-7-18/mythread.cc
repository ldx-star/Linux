#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include<cstring>

using namespace std;

void *startRoutine(void* args){
    pthread_detach(pthread_self());
    while(true){
        cout << (char*)args <<": " << pthread_self()<<  endl;
        // sleep(1);
    }
    return (void *)10;
}


int main(){
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_create(&tid1,nullptr,startRoutine,(void *)"thread1");
    pthread_create(&tid2,nullptr,startRoutine,(void *)"thread2");
    pthread_create(&tid3,nullptr,startRoutine,(void *)"thread3");

    int n = pthread_join(tid1, nullptr);
    cout << n << ":" << strerror(n) << endl;
    n = pthread_join(tid2, nullptr);
    cout << n << ":" << strerror(n) << endl;
    n = pthread_join(tid3, nullptr);
    cout << n << ":" << strerror(n) << endl;

    return 0;
}