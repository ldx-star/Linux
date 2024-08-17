#pragma once
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

namespace Util
{
    bool SetNonBlock(int sock){
        int flag = fcntl(sock, F_GETFL); //获取文件描述符
        if(flag == -1) return false;
        int n = fcntl(sock,F_SETFL, flag | O_NONBLOCK);
        if(n == -1) return false;
        return true;
    }
}