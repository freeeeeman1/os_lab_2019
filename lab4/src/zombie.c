#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	pid_t child_pid;
	/* —оздание дочернего процесса. */
	child_pid = fork();
	if (child_pid > 0)
	{
		/* Ёто родительский процесс Ч делаем минутную паузу. */
		sleep(60);
	}
	else
	{
		/* Ёто дочерний процесс Ч немедленно завершаем работу. */
		exit(0);
	}
	return 0;

}