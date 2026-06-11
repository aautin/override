#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *buffer_0x70;    // rbp-70  -> username input buffer
    char *buffer_0xa0;    // rbp-a0  -> password buffer
    char *buffer_0x110;   // rbp-110 -> password input buffer

    memset(buffer_0x70,  0, 12);
    memset(buffer_0xa0,  0, 5);
    memset(buffer_0x110, 0, 12);

    int fd_0x8 = fopen("/home/users/level03/.pass", "r"); // rbp-0x8
    if (fd_0x8 == NULL)
    {
        fwrite("ERROR: failed to open password file\n", 1, 0x24, stderr); // 41
        return 1;
    }

    int read_0xc = fread(buffer_0xa0, 1, 0x29, fd_0x8); // 41
    if (read_0xc != 0x29) // 41
    {
        fwrite("ERROR: failed to read password file\n", 1, 0x24, stderr);
        return 1;
    }

    buffer_0xa0[strcspn(buffer_0xa0, "\n")] = 0;

    puts("===== [ Secure Access System v1.0 ] =====");
    puts("...");
    puts("...");
    
    printf("--[ Username: ");

    fgets(buffer_0x70, 0x64, stdin); // 0x64 = 100
    buffer_0x70[strcspn(buffer_0x70, "\n")] = 0;

    fgets(buffer_0x110, 0x64, stdin);
    buffer_0x110[strcspn(buffer_0x110, "\n")] = 0;

    puts("*****************************************");

    if (strncmp(buffer_0xa0, buffer_0x110, 0x29)) // 41
    {
        printf(buffer_0x70);
        puts(" does not have access!");
        exit(1);
    }

    printf("Greetings, %s!\n", buffer_0x70);
    system("/bin/sh");
    
    return 0;
}