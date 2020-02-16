#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * @file shell.c
 *
 * @brief  A simple shell
 *
 * @author Clay Buxton
 *
 */


char* prompt = "> ";
pid_t runningPid; //The pid of the most recently run command. 
pid_t shellPid;   //The pid of the shell
int totalCommands = 2; //The amount of defined commands in the shell
char* commandIndex[2] = {"exit","prompt"}; //The names of those commands
void (*commandPointers[2])() = {&cmd_exit,&cmd_prompt}; //The pointers to the functions of the commands

/**
* @brief The "exit" command. Closes to the shell
*/
void cmd_exit(void){
    printf("Goodbye\n");
    exit(0);
}

/**
* @brief The "prompt" command. Changes the prompt
*/
void cmd_prompt(void){
    printf("Change prompt to: ");
    prompt = readLine();
}


/**
* @brief Reads a line in from the user
*
* @returns the line read in from the user
*/
char* readLine(void) {
    int size = 512;
    int pos = 0;
    int c;

    char *line = malloc(sizeof(char) * size);

    while (1) {
        c = getchar();

        //Make bigger if needed
        if (pos == size) {
            size += 512;
            line = realloc(line, size);
        }

        //End it once eof is reached
        if (c == '\n' || c == EOF) {
            line[pos] = '\0';
            return line;
        //Otherwise keep adding
        } else {
            line[pos] = c;
        }

        pos++;
    }
}

/**
* @param line the line read in from the user
* @returns an Argument with the tokens from the line and the length of the line
*
* Takes the line in and tokenizes it
*
*/
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
        //Make bigger if need be, increase a len-1 because the final element has to be NULL, and it needs to have room
        //for that final element
        if (pos == output.len - 1) {
            output.len += 32;
            output.tokens = realloc(output.tokens, output.len);
        }
        token = strtok(NULL, " ");
        output.tokens[pos] = token;
    }
    //NULL terminate
    output.tokens[pos] = NULL;
    pos++;
    output.len = pos;

    return output;
}

/**
* @param args The arguments to execute
* @param runInBackground if the command should run in the background
*
* @brief Runs the commands passed to it
*/
int exec(Arguments args, int runInBackground) {

    pid_t shellPid;

    runningPid = fork(); //For process

    //If something goes wrong
    if(runningPid == -1){
        printf("Catastrophic Failure\n");
        return -1;
    }

    //The Child
    if(runningPid == 0){
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
        waitpid(runningPid,NULL,0);
    }
    //Run in background if the & was found
    else{
        printf("New child process PID: %d\n",runningPid);
    }
}

/**
* @param key the command to search for
* @returns the positon in the function pointer array to execute
* @brief finds a command in the local commands
*/
int findCommand(char* key){
    if(key == NULL){
        return -2;
    }
    for(int i = 0; i < totalCommands; i++){
        if(strcmp(commandIndex[i],key) == 0){
            return i;
        }
    }
    return -1;
}

/**
* @param args the arguemtns to be handeld
* @brief Handles commands to make sure they're executed correctly. Handles &'s and local commands
*
*/
int handleCommands(Arguments args) {
    int runInBackground = 0;
    int definedCommand = -1;

    definedCommand = findCommand(args.tokens[0]);
    //Fix this spahget
    if(definedCommand == -2){
        return -1;
    }
    if(definedCommand != -1){
        (*commandPointers[definedCommand])();
    }
    else{

        if(strcmp(args.tokens[args.len-2],"&") == 0){
            runInBackground = 1;
            //Cut off the & at the end so it doesn't get executed. Just move NULL up one
            args.tokens[args.len-2] = NULL;
            args.len--;
        }
        exec(args, runInBackground);
    }
}

/**
* @brief hanldes the sigint
*
*/
void sigintHandler(){
    kill(runningPid, SIGKILL);
}

/**
*
* @brief initalizes the shell
*
*/
void init(){
    printf("Shell Program: CS422\n----------------\n");
    signal(SIGINT,sigintHandler);
}


/**
* @param filename the file to open
* @brief opens a file and runs all of the commands described inside
*/
int runFile(char* filename){
    FILE* file;
    char* line;
    size_t len = 0;
    ssize_t read;

    file = fopen(filename,"r");
    if(file == NULL){
        printf("Invalid Flie");
        return 1;
    }

    while ((read = getline(&line, &len, file)) != -1) {
        int end = strlen(line) -1;
        //Chop off the newline at the end
        if(line[end] == '\n'){
            line[end] = '\0';
        }
        Arguments fileArgs = getArgs(line);
        handleCommands(fileArgs);
    }

    fclose(file);

}

/**
*
* @brief main
*
*/
int main(int argc, char** argv) {
    char *line;
    char **args;
    
    shellPid = getpid();
    printf("Parent PID: %d\n",shellPid);

    init();

    if(argc > 1){
        runFile(argv[1]);
    }

    //Main loop
    while (1) {    
        printf("%s",prompt);
        line = readLine();
        Arguments args = getArgs(line);
        handleCommands(args);
    }
}
