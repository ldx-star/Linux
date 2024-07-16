#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <cstring>
#include <cerrno>
#include <sys/shm.h>
#include <unistd.h>
using namespace std;

#define PATH_NAME "/home/ldx"
#define PROJ_ID 0x14

#define MEM_SIZE 4096

const int FLAG = IPC_CREAT | IPC_EXCL;

key_t CreateKey(){
    key_t key = ftok(PATH_NAME,PROJ_ID);
    if(key < 0){
        cerr << "ftok error: " << strerror(errno) << endl;
        exit(1);
    }
    
    return key;
}