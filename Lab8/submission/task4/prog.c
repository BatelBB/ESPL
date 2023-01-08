#include <stdio.h>

int main(int argc, char *argv[])
{
    const char *str = argv[1];
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            count++;
        }
    }

    printf("%d\n", count);
    return 0;
}