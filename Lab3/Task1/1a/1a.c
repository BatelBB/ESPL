#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus
{
    unsigned short SigSize;
    char virusName[16];
    unsigned char *sig;
} virus;

// this function receives a file pointer and returns a virus* that represents the next virus in the file.
virus *readVirus(FILE *file)
{
    virus *v = malloc(sizeof(virus));


    fread(&v->SigSize,sizeof(unsigned short),1,file);
    fread(v->virusName,sizeof(char),16,file);
    v->sig = malloc(v->SigSize);
    fread(v->sig,sizeof(unsigned char),v->SigSize,file);

    
    return v;
}

void printHex(unsigned char *buffer, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%X ", buffer[i]);
    }
    printf("\n");
}

// This function receives a virus and a pointer to an output file.
// The function prints the virus to the given output.
// It prints the virus name (in ASCII), the virus signature length (in decimal),
// and the virus signature (in hexadecimal representation).
void printVirus(virus *vi, FILE *output)
{

    fprintf(output, "Name: %s, Length: %d, Signature: ", vi->virusName, vi->SigSize);
    printHex(vi->sig,vi->SigSize);
}

int main(int argc, char **argv)
{
    // if (argc < 2)
    // {
    //     fprintf(stderr, "Missing arguments\n");
    //     exit(3);
    // }

    FILE *fp = fopen("signatures-L", "rb");
    

    char four_first_bytes[4];
    fread(four_first_bytes, sizeof(char), 4, fp);
    if (fp == NULL)
    {
        fprintf(stderr, "File doesn't exist\n");
        exit(3);
    }
    char readings;
    virus *thisIsTheVirus;
    readings = fgetc(fp);

    
    while (readings != EOF)
    {
        readings = ungetc(readings, fp);
        thisIsTheVirus = readVirus(fp);
        printVirus(thisIsTheVirus, stdout);
        free(thisIsTheVirus);

        readings = fgetc(fp);
    }
    fclose(fp);
    return 0;
}