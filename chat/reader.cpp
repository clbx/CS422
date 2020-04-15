#include "chat.h"
  
char* str;
chatroom* chatptr;
int host;

void handler(int signum){
    if(chatptr->type == 0){
        if(host){
            for(int pid : chatptr->readers){
                kill(pid,SIGUSR1);
            }
        }
        std::cout << chatptr->senderName << " [" << chatptr->sender << "]: ";
        for(int i = chatptr->lowIdx; i <= chatptr->highIdx; i++){
            printf("%c",chatptr->buff[i]);
        }
        printf("\n");
    }
    else if(chatptr-> type == 1){
        if(host){
            for(int pid : chatptr->readers){
                kill(pid,SIGUSR1);
            }
        }
        printf("User %d has logged on\n",chatptr->sender);
        chatptr->writers.push_back(chatptr->sender);
    }
    else if(chatptr-> type == 2){
        if(host){
            for(int pid : chatptr->readers){
                kill(pid,SIGUSR1);
            }
        }
        printf("User %d has logged off\n",chatptr->sender);
        chatptr->writers.erase(std::remove(chatptr->writers.begin(), chatptr->writers.end(), chatptr->sender),chatptr->writers.end());
    }
    //Should only be processed by host
    else if(chatptr->type == 3){
        if(!host){
            printf("Signal received that should only host should receive\n");
        }
        chatptr->readers.push_back(chatptr->sender);
        printf("Added Reader PID: %d\n",chatptr->sender);
    }
    else if(chatptr-> type == 4){
        if(!host){
            printf("Signal received that should only host should receive\n");
        }
        chatptr->readers.erase(std::remove(chatptr->readers.begin(), chatptr->readers.end(), chatptr->sender),chatptr->readers.end());
        printf("Removed Reader PID: %d\n",chatptr->sender);
    }
    else{
        printf("Unknown Message Received from: %d",chatptr->sender);
    }

} 




int main() 
{ 

    signal(SIGUSR1,handler);

    //Key for the chat
    key_t key = 112233;

    //Check if shared memory exists

    //If so attatch to it
    //If not create it

    host = 0;
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
    
    chatptr = (chatroom*)shmat(shmid,0,0);
    printf("Attatched to shared memory ID:%d\n",shmid);

    printf("Chat viewer running ");
    if(host){
        chatptr->host = getpid();
        printf("[Host]\n");
    }else{
        chatptr->sender = getpid();
        chatptr->type = 3;
        kill(chatptr->host,SIGUSR1);
        printf("\n");
    }
    printf("Host PID: %d\n",chatptr->host);

    int quit = 1;
    char ch;

    while(quit){
        ch = getchar();
        if(ch == 'q'){
            chatptr->sender = getpid();
            chatptr->type = 4;
            kill(chatptr->host,SIGUSR1);
            quit = 0;
        }
    }


    if(host){
        shmctl(shmid,IPC_RMID,NULL);
    }
    printf("Killed Shared Memory ID:%d\n",shmid);


    return 0;
}