#include <unistd.h>
#include <stdio.h>

int main(void)
{
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		printf("\nhello i am child, now i will execute date\n");
		execlp("/bin/date", "/bin/date", (char *)0);
	}
	else if (pid > 0)
		printf("\nhello i am parent\n");
}
