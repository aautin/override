#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	main(void)
{
	char buf[100];
	fgets(&buf, 100, stdin);

	int i = 0;
	while (buf[i] != '\0')
	{
		if (buf[i] >= 'A' && buf[i] <= 'Z')
		{
			buf[i] += 0x20; // 'a' - 'A' = 0x20
		}
		i++;
	}

	printf(buf);
	exit(0);
}