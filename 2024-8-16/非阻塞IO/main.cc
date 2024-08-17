#include "util.hpp"
#include <string>
#include <unistd.h>
using namespace std;
using namespace Util;

int main(){
    string s;
    if(!SetNonBlock(0)){
        cerr << "set nonblock false" << endl;
    }
    while(true){
        cin >> s;
        cout <<"刚刚获取的数据是：" << s << endl;    
        sleep(1);
    }
     
}