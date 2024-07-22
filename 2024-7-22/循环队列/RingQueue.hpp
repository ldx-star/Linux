#pragma once

#include<iostream>
#include<vector>
#include<cstdio>
#include<semaphore.h>
#include<unistd.h>
#include<pthread.h>
using namespace std;

int gCap = 5;

template<class T>
class RingQueue{
public:
RingQueue(int cap = gCap)
    :ringQueue_(cap),cIndex_(0),pIndex_(0)
{   
    //信号量初始化
    sem_init(&roomSem_,0,5);
    sem_init(&dataSem_,0,0);

    pthread_mutex_init(&pmutex_,nullptr);
    pthread_mutex_init(&cmutex_,nullptr);
}
~RingQueue(){
    sem_destroy(&roomSem_);
    sem_destroy(&dataSem_);

    pthread_mutex_destroy(&pmutex_);
    pthread_mutex_destroy(&cmutex_);
}

void Push(const T &in){
    sem_wait(&roomSem_);

    pthread_mutex_lock(&pmutex_);
    ringQueue_[pIndex_] = in;
    pIndex_++;
    pIndex_ %= ringQueue_.size();
    pthread_mutex_unlock(&pmutex_);

    sem_post(&dataSem_);

}

T Pop(){
    sem_wait(&dataSem_);
    pthread_mutex_lock(&cmutex_);

    T tmp = ringQueue_[cIndex_];
    sem_post(&roomSem_);
    cIndex_++;
    cIndex_ %= ringQueue_.size();

    pthread_mutex_unlock(&cmutex_);
    sem_post(&roomSem_);

    return tmp;
}


private:
    vector<T> ringQueue_; //循环队列
    sem_t roomSem_; //空间信号量
    sem_t dataSem_; //资源信号量
    uint32_t cIndex_; //消费者下标
    uint32_t pIndex_; //生产者下标
    pthread_mutex_t pmutex_;
    pthread_mutex_t cmutex_;
};