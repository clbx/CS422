#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int pid;

int main(){
	pid = getpid();
	fork();

	for(int i = 0; i < 10; i++){
		printf("Running process: %d\n",pid);
	}

	return 0;
}
