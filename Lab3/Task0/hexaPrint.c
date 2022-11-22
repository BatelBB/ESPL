#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

// prints length bytes from memory location buffer, in hexadecimal format.
void printHex(unsigned char *buffer, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%X ", buffer[i]);
    }
    printf("\n");
}

// hexaPrint receives, as a command-line argument, the name of a "binary" file,
// and prints the hexadecimal value of each byte to the standard output, separated by spaces.
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Missing arguments\n");
        exit(3);
    }

    int length;
    FILE *fp = fopen(argv[1],"rb");
    unsigned char* buffer;
    size_t result;

    fseek(fp , 0 , SEEK_END);
    length = ftell(fp);
    rewind(fp);

    buffer = (unsigned char*) malloc (sizeof(char)*length);
    result = fread(buffer,1,length,fp);

    printHex(buffer,result);

    fclose(fp);
    free (buffer);
    return 0;
}