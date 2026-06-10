#include <stdio.h>
#include <stdlib.h>

char* header1 = "***********************************";
char* header2 = "* \t     -Level00 -\t\t  *";

int main()
{
    puts(header1);
    puts(header2);
    puts(header1);

    printf("Password:");
    int result;
    scanf("%d", &result);

    if (result == 5276)
	{
        puts("\nAuthenticated!");
        system("/bin/sh");
        return 0;
    }

    puts("\nInvalid Password!");
    return 1;
}