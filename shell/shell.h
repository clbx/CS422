typedef struct Arguments {
    char** tokens;
    int len;
} Arguments;


/* Defined Functions */
void cmd_exit();
void cmd_prompt();

char *readLine(void);
Arguments getArgs(char *line);
int exec(Arguments args, int runInBackground);
int handleCommands(Arguments args);