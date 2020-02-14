#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "shell.h"


/**
void main(int argc, char* argv[]) {
    int i,status;
    for (i=0; i<=argc; i++) {
        printf("arg %d: %s\n",i,argv[i]);
        myargs[0] = argv[i+1];
        myargs[1] = NULL;
        pid = fork();
        if (pid==0) {
                execvp(myargs[0],myargs);
                printf("ERROR!! Your exec call FAILED!!!!\n\n");
            }
        waitpid(pid,&status,0);
        printf("Child terminated, status: %d\n\n",status);
    }
}
*/


char* readLine(void){
    int size = 512;
    int pos = 0;
    int c;

    char* line = malloc(sizeof(char) * size);
    

    while(1){
        c = getchar();
        
        
        if(pos == size){
            size += 512;
            line = realloc(line, size);
        }

        if(c == '\n' || c == EOF){
            line[pos] = '\0';
            return line;
        }else{
            
            line[pos] = c;
        }

        pos++;
    }
}


Arguments getArgs(char* line){
    int pos = 0;
    Arguments output;
    output.len = 32;
    output.tokens = malloc(sizeof(char*) * output.len);
    char** tokens = malloc(sizeof(char*) * output.len);
    

    char* token = strtok(line," ");
    output.tokens[pos] = token;
    while(token){
        pos++;
        if(pos == output.len){
            output.len += 32;
            output.tokens = realloc(output.tokens,output.len);
        }
        token = strtok(NULL," ");
        output.tokens[pos] = token;
        
    }
    output.len = pos;

    return output;
}




int main(){
    int running = 1;
    int failed;
    char* line;
    char** args;
    int status, pid;
    printf("CS422 Shell\n");
    while(running){
        printf("> ");
        line = readLine();
        Arguments args = getArgs(line);
        pid = fork();
        if (pid==0) {
                execvp(args.tokens[0],args.tokens);
                printf("Execution Failed ");
            }
        waitpid(pid,&status,0);
        if(status){
            printf("[%d]\n",status);
        }
        
        

    }
}
