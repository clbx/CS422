#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <string>
#include <pwd.h>
#include <algorithm>

struct chatroom{
    int host; //PID of the process that established memory
    std::vector<int> readers; //Stores PIDs of all the connected readers
    std::vector<int> writers; //Stores UIDs of all the connected writers
    char buff[16000]; //Where the messages are stored
    int lowIdx; //The lower bound of the most recent message
    int highIdx; //The upper bound of the most recent message
    int sender; //The sender of the most recent message
    std::string senderName; //The sender's name
    int type; //The type of message being sent
    /**
     * 0: Normal Message
     * 1: Writer Logon
     * 2: Writer Logoff
     * 3: Reader Logon
     * 4: Reader Logoff
     */
} typedef chatroom;