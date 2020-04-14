#include "chat.h"
  
char* str;

void handler(int signum){
    printf("Received Message\n");
} 




int main() 
{ 

    signal(SIGUSR1,handler);

    //Key for the chat
    key_t key = 112233;

    //Check if shared memory exists

    //If so attatch to it
    //If not create it

    int host = 0;
    int shmid;

    shmid = shmget(9000, sizeof(chatroom), IPC_EXCL | IPC_CREAT |0666);
    //If shmid is -1, a shared memory instance already exists, we need to attach to it
    if(shmid < 0){
        printf("A shared memory instance already exists attatching...\n");
        shmid = shmget(9000,sizeof(chatroom),IPC_CREAT |0666);
    }
    else{
        printf("New shared memory instance created ID:%d\n",shmid);
        host = 1;
    }
    
    chatroom* chatptr = (chatroom*)shmat(shmid,0,0);
    printf("Attatched to shared memory ID:%d\n",shmid);

    printf("Chat viewer running");
    if(host){
        chatptr->pid = getpid();
        printf(" [Host]\n");
    }else{
        printf("\n");
    }
    printf("Host PID: %d\n",chatptr->pid);

    int quit = 1;
    char ch;

    while(quit){
        ch = getchar();
        if(ch == 'q'){
            quit = 0;
        }
    }


    if(host){
        shmctl(shmid,IPC_RMID,NULL);
    }
    printf("Killed Shared Memory ID:%d\n",shmid);


    return 0;
}