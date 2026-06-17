#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/ptrace.h>

int auth(char *login, unsigned int serial)
{
	int          len;
	unsigned int hash;

	login[strcspn(login, "\n")] = 0;
	len = strnlen(login, 32);

	if (len < 6)
	{
		return EXIT_FAILURE;
	}

	if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1)
	{
		puts("CONTEXT STRINGS");
		return EXIT_FAILURE;
	}

	hash = (login[3] ^ 4919) + 6221293;

	for(int i = 0; i < len; i++)
	{
		if (login[i] < 32)
		{
			return EXIT_FAILURE;
		}

		hash += (login[i] ^ hash) % 1337;
	}

	return serial != hash;
}

int main()
{
	unsigned int serial;
	char		 login[32];

	puts("CONTEXT STRINGS");
	printf("-> Enter Login: ");
	fgets(login, 32, stdin);
	
	puts("CONTEXT STRINGS");
	scanf("%u", &serial);

	if (!auth(login, serial))
	{
		puts("Authenticated!");
		system("/bin/sh");

		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}