#include "chat.h"
chatroom* chatptr;



int main() { 
    // ftok to generate unique key 
    key_t key = 112233;
    int user = 10;
    int shmid;
    shmid = shmget(9000, sizeof(chatroom), IPC_EXCL | IPC_CREAT |0666);

    if(shmid < 0){
        printf("Found Shared Memory Instance\n");
        shmid = shmget(9000,sizeof(chatroom),IPC_CREAT |0666);
    }
    else{
        printf("Could not find any shared memory instance. Exiting...");
        return 1;
    }
    chatptr = (chatroom*)shmat(shmid,0,0);
    printf("Host PID: %d",chatptr->host);
    chatptr->sender = getuid();
    chatptr->type = 1;
    kill(chatptr->host, SIGUSR1);
    std::string buf;
    char ch;
    printf("\n");
    while(1){
        std::cout << getlogin() << ": ";
        std::getline(std::cin,buf);
        if(buf == "/logout"){
            chatptr->sender = getuid();
            chatptr->senderName = getlogin();
            chatptr->type = 2;
            kill(chatptr->host,SIGUSR1);
            break;
        }
        else if(buf == "/users"){
            printf("This will print users soon\n");
        }
        else{
            chatptr-> type = 0;
            chatptr-> sender = getuid();
            chatptr-> senderName = getlogin();
            int tmp = chatptr->highIdx;
            for(int i = 0; i < buf.size(); i++){
                chatptr->buff[chatptr->highIdx+1+i] = buf[i];
            }
            chatptr->lowIdx = tmp+1;
            chatptr->highIdx = chatptr->lowIdx+buf.size();
            kill(chatptr->host,SIGUSR1);
        }
    }
    return 0; 
} 