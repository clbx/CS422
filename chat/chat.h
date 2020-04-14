#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

struct chatroom{
    int pid; //PID of the process that established memory
    char buff[16000];
} typedef chatroom;