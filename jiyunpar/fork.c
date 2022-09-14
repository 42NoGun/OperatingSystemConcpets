#include <unistd.h>
#include <stdio.h>

int main(void)
{
	int pid;

	pid = fork();
	if (pid == 0)
		printf("\nhello, i am child\n");
	else if (pid > 0)
		printf("\nhello, i am parent\n");
		
}
