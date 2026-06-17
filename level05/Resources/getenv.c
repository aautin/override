#include <stdio.h>
#include <stdlib.h>

int main()
{
	printf("SHELLCODE: %p\n", getenv("SHELLCODE"));
}