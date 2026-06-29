#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void clear_stdin()
{
	char c = getchar();
	while (c != -1)
	{
		if (c == "\n")
		{
			break;
		}

		c = getchar();
	}
}

unsigned int get_unum()
{
	unsigned int input;

	fflush(stdout);

	scanf("%u", input);

	clear_stdin();

	return input;
}

int store_number(unsigned int *data)
{
	unsigned int input;
	unsigned int index;

	printf(" Number: ");
	input = get_unum();

	printf(" Index: ");
	index = get_unum();

	if (0 == index % 3 || (index >> 0x18 == 0xb7))
	{
		printf(" *** ERROR! ***\n");
		printf("   This index is reserved for wil!\n");
		printf(" *** ERROR! ***\n");

		return 1;
	}

	data[index] = input;

	return 0;
}


int read_number(unsigned int *data)
{
	unsigned int index;

	printf(" Index: ");
	index = get_unum();

	printf(" Number at data[%u] is %u\n", index, data[index]);

	return 0;
}

int main(int argc, char *argv[], char *env[])
{
	int          result    = 0;
	char         cmd[20]   = {0};
	unsigned int data[100] = {0};

	while (*argv)
	{
		memset(*argv, 0, strlen(*argv) - 1);
		*argv++;
	}

	while (*env)
	{
		memset(*env, 0, strlen(*env) - 1);
		*env++;
	}

	puts("CONTEXT STRING");

	while (true)
	{
		printf("Input command: ");
		result = 1;

		fgets(cmd, sizeof(cmd), stdin);
		cmd[strlen(cmd) - 1] = '\0';

		if (!strncmp(cmd, "store", 5))
		{
			result = store_number(data);
		}
		else if (!strncmp(cmd, "read", 4))
		{
			result = read_number(data);
		}
		else if (!strncmp(cmd, "quit", 4))
		{
			break;
		}

		if (result)
		{
			printf(" Failed to do %s command\n", cmd);
		}
		else
		{
			printf(" Completed %s command successfully\n", cmd);
		}

		memset(cmd, 0, sizeof(cmd));
	}

	return EXIT_SUCCESS;
}