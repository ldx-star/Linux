#pragma once

#include<iostream>
#include<queue>
#include<cstdio>
#include <ctime>
#include<unistd.h>
using namespace std;

const uint32_t gDefaultCap = 5;

template <class T>
class BlockQueue{
public:
    BlockQueue(uint32_t cap = gDefaultCap)
        :cap_(cap)
    {
        pthread_mutex_init(&mutex_,nullptr);
        pthread_cond_init(&conCond_,nullptr);
        pthread_cond_init(&proCond_,nullptr);
    }
    ~BlockQueue(){
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&conCond_);
        pthread_cond_destroy(&proCond_);
    }
public:
    // 生产接口
    void push(const T &in){
        //加锁
        lockQueue();
        //如果队列满了，就等待
        while(isFull()){
            proBlockWait(); //阻塞等待
        }
        //条件满足，可以生产
        bq_.push(in);
        //解锁
        unlockQueue();
        //唤醒消费者
        wakeupCon();
    }

    // 消费接口
    T pop(){
        //加锁
        lockQueue();
        
        //如果队列为空，就等待
        while(isEmpty()){
            conBlockWait(); //阻塞等待
        }
        //条件满足，可以生产
        T front =  bq_.front();
        bq_.pop();
        //解锁
        unlockQueue();
        //唤醒生产者
        wakeupPro();
        return front;
    }

private:
    void lockQueue(){
        pthread_mutex_lock(&mutex_);
    }
    void unlockQueue(){
        pthread_mutex_unlock(&mutex_);
    }
    bool isFull(){
        return bq_.size() >= cap_;
    }
    bool isEmpty(){
        return bq_.empty();
    }
    void conBlockWait(){
        // 在阻塞等待的时候会自动释放锁
        pthread_cond_wait(&conCond_,&mutex_);
        // 当等待结束的时候，该线程醒来时会重新的到锁 
    }
    void proBlockWait(){
        pthread_cond_wait(&proCond_,&mutex_);
    }
    void wakeupCon(){
        pthread_cond_signal(&conCond_);
    }
    void wakeupPro(){
        pthread_cond_signal(&proCond_);
    }

private:
    uint32_t cap_; // 容量
    queue<T> bq_; // blockqueue
    pthread_mutex_t mutex_; // 互斥锁
    pthread_cond_t conCond_; // 让消费者等待的条件变量
    pthread_cond_t proCond_; // 让生产者等待的条件变量 
};