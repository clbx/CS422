#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char* prompt = "> ";

int totalCommands = 2;
char** commandIndex = {"exit","prompt"};
void (*commandPointers[3])() = {&cmd_exit,&cmd_prompt};


void cmd_exit(){
    printf("Goodbye\n");
    exit(0);
}

void cmd_prompt(){
    printf("Change prompt to: ");
    prompt = readLine();
}

char *readLine(void) {
    int size = 512;
    int pos = 0;
    int c;

    char *line = malloc(sizeof(char) * size);

    while (1) {
        c = getchar();

        if (pos == size) {
            size += 512;
            line = realloc(line, size);
        }

        if (c == '\n' || c == EOF) {
            line[pos] = '\0';
            return line;
        } else {
            line[pos] = c;
        }

        pos++;
    }
}

Arguments getArgs(char *line) {
    int pos = 0;
    Arguments output;
    output.len = 32;
    output.tokens = malloc(sizeof(char *) * output.len);
    char **tokens = malloc(sizeof(char *) * output.len);

    char *token = strtok(line, " ");
    output.tokens[pos] = token;
    while (token) {
        pos++;
        if (pos == output.len - 1) {
            output.len += 32;
            output.tokens = realloc(output.tokens, output.len);
        }
        token = strtok(NULL, " ");
        output.tokens[pos] = token;
    }
    output.tokens[pos] = NULL;
    pos++;
    output.len = pos;

    return output;
}

int exec(Arguments args, int runInBackground) {

    pid_t pid, shellPid;

    pid = fork();

    if(pid == -1){
        printf("Catastrophic Failure\n");
        return -1;
    }

    //The Child
    if(pid == 0){
        signal(SIGINT, SIG_IGN); //Ignore SIGINT
        if(execvp(args.tokens[0],args.tokens) == -1){
            //Terminate the child if the command is bad
            printf("Command not Found\n");
            kill(getpid(),SIGTERM); 
        }
    }

    //The Parent

    //If it's not running in the background wait for the child
    if(runInBackground == 0){
        waitpid(pid,NULL,0);
    }
    //Run in background if the & was found
    else{
        printf("New child process PID: %d\n",pid);
    }
}

int handleCommands(Arguments args) {
    int runInBackground = 0;
    int definedCommand = 0;


    if(strcmp(args.tokens[args.len-2],"&") == 0){
        runInBackground = 1;

        args.tokens[args.len-2] = NULL;
        args.len--;
    }
    exec(args, runInBackground);

}

int main() {
    int running = 1;
    char *line;
    char **args;
    pid_t shellPid;

    printf("CS422 Shell\n");
    while (running) {
        shellPid = getpid();
        printf("%s",prompt);
        line = readLine();
        Arguments args = getArgs(line);
        handleCommands(args);
    }
}
