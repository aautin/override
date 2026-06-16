#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/prctl.h>
#include <signal.h>

void asm_builtin_memset(char *s, char c, size_t n)
{
	for (size_t i = 0; i < n; i++)
	{
		s[i] = c;
	}
}

int	main(void)
{
	pid_t	pid;
	char	s[32];
	
	pid = fork();
	asm_builtin_memset(s, 0, 32);
	
	int32_t	v       = 0;
	long	wstatus = 0;

	if (!pid)
	{
		prctl(PR_SET_PDEATHSIG, SIGHUP);
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		puts("Give me some shellcode, k");
		gets(s);

		return EXIT_SUCCESS;
	}

	while (true)
	{
		wait(wstatus);
		if ((wstatus & 0x7f) == 0)
		{
			puts("child is exiting...");

			return EXIT_SUCCESS;
		}
		
		v = ptrace(PTRACE_PEEKUSER, pid, 44, 0);
		if (v == 11)
		{
			puts("no exec() for you");
			kill(pid, 9);
			
			return EXIT_SUCCESS;
		}
	}

	return EXIT_SUCCESS;
}