#include "ThreadPool.hpp"
#include "Task.hpp"

int main(){
    ThreadPool<Task> tp;
    tp.start();
    const string operators = "+-*/";
    srand((unsigned long)time(nullptr));
    while(true){
        int one = rand() % 50;
        int two = rand() % 20;
        char op = operators[rand()%4];
        cout << "主线程派发任务：" << one << op << two << endl;
        Task t(one,two,op);
        tp.push(t);
        sleep(1);
    }
    return 0;
}