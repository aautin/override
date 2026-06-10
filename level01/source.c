#include <stdio.h>
#include <stdlib.h>

char username_buffer[256];

int verify_user_name()
{
    puts("verifying username....\n");
    return strncmp("dat_wil", username_buffer, 7);
}

int verify_user_pass(char* buffer)
{
    return strncmp("admin", buffer, 5);
}

int main() {
    char buffer[64]; // 0x5c - 0x1c = 0x40

    memset(buffer, 0, 64);

    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");
    fgets(username_buffer, 0x100, stdin);

    if (verify_user_name())
    {
        puts("nope, incorrect username...\n");
        return 1;
    }

    puts("Enter Password: ");
    fgets(buffer, 100, stdin);

    if (verify_user_pass(buffer) || 0)
    {
        puts("nope, incorrect password...\n");
        return 1;
    }
    
    return 0;
}