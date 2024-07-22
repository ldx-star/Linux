#include"BlockQueue.hpp"
#include "Task.hpp"

void *consumer(void *args){
    BlockQueue<Task> *bq =static_cast<BlockQueue<Task> *> (args);
    while(true){
        Task t = bq->pop();
        cout << t.run() << endl;
        sleep(2);
    }
}
void *productor(void *args){
    BlockQueue<Task> *bq =static_cast<BlockQueue<Task> *> (args);
    while(true){
        // 1、制作数据 
        int a = rand() % 10;
        int b = rand() % 5;
        int c = rand() % 4;
        char op;
        if(c == 0){
            op = '+';
        }
        if(c == 1){
            op = '-';
        }
        if(c == 2){
            op = '*';
        }
        if(c == 3){
            op = '/';
        }
        // 2、生产数据
        Task t(a,b,op);
        bq->push(t);
        cout << a << op << b << endl;

        //        sleep(2);
    }

}

int main(){
    srand((unsigned long) time(nullptr));

    BlockQueue<Task> bq;

    pthread_t c,p;
    pthread_create(&c,nullptr,consumer,&bq);
    pthread_create(&p,nullptr,productor,&bq);

    pthread_join(c,nullptr);
    pthread_join(p,nullptr);
    return 0;

}