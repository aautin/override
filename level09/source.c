#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

typedef struct s_struct
{
	char msg[140];
	char username[40];
	int  msg_length;
}	t_struct;

void secret_backdoor()
{
	char command[128];

	fgets(command, 0x80, stdin); // 128
	system(command);
}

void handle_msg()
{
	t_struct structure;

	memset(structure.username, 0, 40);
	structure.msg_length = 0x8c; // 140

	set_username(&structure);
	set_msg(&structure);

	puts("CONTEXT STRINGS");
}

void set_msg(t_struct *structure)
{
	char buffer[1024];

	memset(buffer, 0, 1024);

	puts("CONTEXT STRINGS");

	fgets(buffer, 1024, stdin);

	strncpy(structure->msg, buffer, structure->msg_length);
}

void set_username(t_struct *structure)
{
	char buffer[128];

	void** ptr = buffer; // void* is QWORD
	
	ptr[0] = 0;
	ptr[1] = 0;
	ptr[2] = 0;
	ptr[3] = 0;
	ptr[4] = 0;

	puts("CONTEXT STRINGS");
	
	fgets(buffer, 128, stdin);

	for(int i = 0; i <= 40 && buffer[i]; i++)
	{
		structure->username[i] = buffer[i];
	}

	puts("CONTEXT STRINGS");
}

int main()
{
  	puts("CONTEXT STRINGS");

	handle_msg();

	return 0;
}
