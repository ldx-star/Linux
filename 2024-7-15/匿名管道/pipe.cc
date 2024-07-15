#include<iostream>
#include<cstdio>
#include<cstring>
#include<stdlib.h>
#include<ctime>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<vector>
#include<cassert>

using namespace std;

typedef void(*functor)(); // 函数指针
vector<functor> functors;

void f1(){
    cout << "这是一个处理日志的任务，执行的进程ID:[" << getpid() << "]" << "执行的时间是[" << time(nullptr) <<"]" << endl;
}
void f2(){
    cout << "这是一个数据备份的任务，执行的进程ID:[" << getpid() << "]" << "执行的时间是[" << time(nullptr) <<"]" << endl;
}
void f3(){
    cout << "这是一个网络连接的任务，执行的进程ID:[" << getpid() << "]" << "执行的时间是[" << time(nullptr) <<"]" << endl;
}
void loadFunctor(){
    functors.push_back(f1);
    functors.push_back(f2);
    functors.push_back(f3);
}


typedef pair<int32_t,int32_t> elem;
int processNum = 5;

void work(int blockFd){
    cout << "进程[" << getpid() <<"]开始工作" << endl; 
    while(true){
        uint32_t operatorCode = 0;
        ssize_t s = read(blockFd, &operatorCode,sizeof(uint32_t));
        if(s == 0) break;
        assert(s == sizeof(uint32_t));
        (void)s;

        if(operatorCode < functors.size()) functors[operatorCode]();
    }
    cout << "进程[" << getpid() <<"]结束工作" << endl; 
}
void sendTasks(const vector<elem> &assignMap){
    
    srand((long long)time(nullptr));

    while(true){
        // 选择一个进程
        int pick = rand() % assignMap.size();
        // 选择一个任务
        uint32_t task = rand() % functors.size();

        // 指派任务
        write(assignMap[pick].second, &task,sizeof(uint32_t));

        cout << "父进程指派任务:" << task << "给进程："<< assignMap[pick].first << "\t编号:" << pick << endl;
        sleep(1);
    }
}

int main(){
    vector<elem> assignMap;
    loadFunctor();
    // 创建processNum个进程
    for(int i = 0; i < processNum; i++){
        // 创建管道
        int pipefd[2] = {0};
        pipe(pipefd);
        // 创建子进程
        pid_t id = fork();
        if(id == 0){
            // 子进程
            close(pipefd[1]);
            work(pipefd[0]);
            exit(0);
        }
        //父进程
        close(pipefd[0]);
        // 将子进程pid与对应管道建立联系
        elem e(id,pipefd[1]);
        assignMap.push_back(e);

    }
    sendTasks(assignMap);
    //回收资源
    for(int i = 0; i < processNum; i++){
        if(waitpid(assignMap[i].first,nullptr,0) > 0){
            cout << "wait for pid=" << assignMap[i].first << "sucess!" << "\tnum:" << i << endl;
            close(assignMap[i].second); 
        }
    }    
}

// int main(){
//     // 加载进程
//     loadFunctor();

//     // 创建管道
//     int pipefd[2] = {0};
//     if(pipe(pipefd) != 0){
//         cerr << "pipe error" << endl;
//         return 1;
//     }
//     // 创建子进程
//     pid_t id =  fork();
//     if(id < 0){
//         cerr << "fork error" << endl;
//         return 2;
//     }else if(id == 0){
//         // 子进程，读
//         close(pipefd[1]);
//         // 业务处理
//         while(true){
//             uint32_t operatorType = 0;
//             ssize_t s = read(pipefd[0], &operatorType, sizeof(uint32_t));
//             if(s == 0){
//                 cout << "没有任务了" << endl;
//                 break;
//             }
//             assert(s == sizeof(uint32_t));
//             // assert是编译是有效，debug模式
//             // 在release模式中assert就没有了
//             // 没有assert之后，s是一个被定义但是没有被使用的变量，编译器会报warning
//             // 使用 (void)s 为了避免warning
//             (void)s;
//             if(operatorType < sizeof(functors)){
//                 functors[operatorType]();
//             }else{
//                 cerr << "bug? operatorTyper = " << operatorType << endl;
//             }
            
//         }
//         close(pipefd[0]);
//         exit(0);
//     }else{
//         srand((long long)time(nullptr));
//         // 父进程，写
//         close(pipefd[0]);
//         // 指派任务
//         int num = functors.size();
//         int cnt = 10;
//         while(cnt--){
//             uint32_t command = rand() % num;
//             write(pipefd[1],&command, sizeof(uint32_t));
//             sleep(1);
//         }

//         close(pipefd[1]);
        
//         pid_t res = waitpid(id,nullptr,0);
//         if(res){
//             cout << "wait success" << endl;
//         }


//     }


//     return 0;
// }