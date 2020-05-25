#include <stdio.h>

#include <ctype.h>
#include <limits.h>
#include <stdbool.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>




int main(int argc, char **argv)
{
	const char* NameScript = "./sequential_min_max";
	char *args[4] = { " ","1","10",NULL};
	int rv;
	

	int pid = fork();

	if (pid == 0) {
		execvp(NameScript, args);
		exit(rv);
	}
	else
	{
		int status;
		wait(&status);
	}	
	
	//wait(2);
	printf("Finished executing the parent process\n"
		" - the child won't get here--you will only see this once\n");


	fflush(NULL);
	return 0;
}
