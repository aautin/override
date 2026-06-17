#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

void log_wrapper(FILE *file, char *context_string, char *filename)
{
  char buffer[264];
  strcpy(buffer, context_string);

  snprintf(buffer + strlen(buffer), 255 - strlen(buffer) - 1, filename);
  buffer[strcspn(buffer, "\n")] = '\0';

  fprintf(file, "LOG: %s\n", buffer);
}

int main(int argc, char *argv[])
{
	char buffer[100];

	FILE *src;
	FILE *log;
	int   backup;

	if (argc != 2)
	{
		printf("Usage: %s filename\n", argv[0]);
	}
	
	log = fopen("./backups/.log", "w");
	if (!log)
	{
		printf("ERROR: Failed to open %s\n", "./backups/.log");
		exit(EXIT_FAILURE);
	}

	log_wrapper(log, "Starting back up: ", argv[1]);

	src = fopen(argv[1], "r");
	if (!src)
	{
		printf("ERROR: Failed to open %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	strcpy(buffer, "./backups/");
	strncat(buffer, argv[1], 100 - strlen(buffer) - 1);
	backup = open(buffer, 193);

	if (backup < 0)
	{
		printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		char character = (char) fgetc(src);

		if (character <= 0)
		{
			// End of file or error
			break;
		}

		write(backup, &character, 1);
	}

	log_wrapper(log, "Finished back up ", argv[1]);
	fclose(src);
	close(backup);

	return EXIT_SUCCESS;
}