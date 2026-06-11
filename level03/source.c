#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void decrypt(int key)
{
    char buf[] = { 0x51,0x7d,0x7c,0x75,
				   0x60,0x73,0x66,0x67,
				   0x7e,0x73,0x66,0x7b,
				   0x7d,0x7c,0x61,0x33,
				   0 };

    for (int i = 0; buf[i]; i++)
	{
        buf[i] ^= key;
	}

    if (!strcmp(buf, "Congratulations!"))
	{
        system("/bin/sh");
	}
    else
	{
		puts("\nInvalid Password");
	}
}

void test(int arg1, int arg2)
{
    int diffBuffer = arg2 - arg1;

    if (diffBuffer > 21)
	{
        decrypt(rand());
	}
    else
	{
        decrypt(diffBuffer);	
	}
}

int main(void)
{
	int input_number;

    srand((unsigned) time(0));

    puts("***********************************");
    puts("*\t\tlevel03\t\t**");
    puts("***********************************");

    printf("Password:");
    scanf("%d", &input_number);

    test(input_number, 0x1337d00d);

    return 0;
}