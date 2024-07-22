#pragma once

#include<iostream>
#include<pthread.h>
#include<queue>
#include<cassert>
#include<unistd.h>
using namespace std;

int gThreadNum = 5;

template <class T>
class ThreadPool{
public:
    ThreadPool(int threadNum = gThreadNum)
        :isStart_(false),threadNum_(threadNum)
    {
        pthread_mutex_init(&mutex_,nullptr);
        pthread_cond_init(&cond_,nullptr);

    }
    ~ThreadPool(){
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&cond_);
    }
public:
    void start(){
        assert(!isStart_);
        for(int i = 0; i < threadNum_; i++){
            pthread_t temp;
            pthread_create(&temp, nullptr, threadRoutine,this);
        }
        isStart_ = true;
    }
    void push(const T &in){
        lockQueue();
        taskQueue_.push(in);
        // 唤醒等待线程
        pthread_cond_signal(&cond_);
        unlockQueue();
    }

private:
// C++中的成员函数默认传入this指针，因此前面要加static
// 静态成员函数不能访问类内成员，需要传入this指针
static void *threadRoutine(void *args){
    // 线程分离，主线程不需要等待
    pthread_detach(pthread_self());

    ThreadPool<T>* tp = static_cast<ThreadPool<T>*>(args);
    while(true){
        // 加锁
        tp->lockQueue();
        // 检测条件
        while(tp->taskEmpty()){
            tp->waitForTask();
        }
        // 获取任务
        T t = tp->pop();        
        // 解锁
        tp->unlockQueue();
        // 执行任务
        int one,two;
        char op;
        t.get(&one,&two,&op);
        cout << "新线程完成任务：" << one << op << two << "=" << t.run() << endl;
    }

}

bool taskEmpty(){
    return taskQueue_.empty();
}

void waitForTask(){
    pthread_cond_wait(&cond_,&mutex_);
}

T pop(){
    T temp = taskQueue_.front();
    taskQueue_.pop();
    return temp;
}

void lockQueue(){pthread_mutex_lock(&mutex_);}
void unlockQueue(){pthread_mutex_unlock(&mutex_);}
private:
    bool isStart_;
    int threadNum_;
    queue<T> taskQueue_;
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
};