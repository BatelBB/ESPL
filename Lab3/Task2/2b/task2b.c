#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus
{
    unsigned short SigSize;
    char virusName[16];
    unsigned char *sig;
} virus;

typedef struct link link;

struct link
{
    link *nextVirus;
    virus *vir;
};
struct fun_desc
{
    char *name;
    link *(*fun)(link *virus_list);
};

FILE *loadFromFile()
{
    char *input = (char *)(calloc(18, sizeof(char)));
    FILE *fp;

    fprintf(stdout, "Enter signature file name:\n");

    fgets(input, 18, stdin);
    sscanf(input, "%s\n", input);
    fp = fopen(input, "rb");
    if (fp == NULL)
    {
        fprintf(stdout, "File doesn't exist\n");
        exit(3);
    }
    free(input);
    return fp;
}
void printHex(unsigned char *buffer, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%X ", buffer[i]);
    }
    printf("\n\n");
}
void printVirus(virus *vi, FILE *output)
{
    fprintf(output, "Virus name: %s\nVirus size: %d\nsignature:\n", vi->virusName, vi->SigSize);
    printHex(vi->sig, vi->SigSize);
}
/* Print the data of every link in list to the given stream. Each item followed by a newline character. */
link *list_print(link *virus_list, FILE *out)
{
    if (virus_list != NULL)
    {
        printVirus(virus_list->vir, out);
        list_print(virus_list->nextVirus, out);
    }
    return virus_list;
}

/* Add a new link with the given data to the list
(either at the end or the beginning, depending on what your TA tells you),
and return a pointer to the list (i.e., the first link in the list).
If the list is null - create a new entry and return a pointer to the entry. */
link *list_append(link *virus_list, virus *data)
{

    if (virus_list == NULL)
    {
        virus_list = malloc(sizeof(link));
        virus_list->vir = NULL;
        virus_list->nextVirus = NULL;
    }
    if (virus_list->vir == NULL)
    {
        // virus_list->vir = (virus *)calloc(1, sizeof(virus));
        virus_list->vir = data;
        return virus_list;
    }
    link *newLink = malloc(sizeof(link));
    newLink->vir = data;
    newLink->nextVirus = NULL;
    if (virus_list == NULL)
    {
        virus_list = newLink;
    }
    else
    {
        link *lastLink = virus_list;

        while (lastLink->nextVirus != NULL)
        {

            lastLink = lastLink->nextVirus;
        }
        lastLink->nextVirus = newLink;
    }
    // free(newLink);
    return virus_list;
}

/* Free the memory allocated by the list. */
link *list_free(link *virus_list)
{
    // From C Wiki book
    if (virus_list != NULL)
    {
        list_free(virus_list->nextVirus);
        free(virus_list->vir->sig);
        free(virus_list->vir);
        free(virus_list);
    }
    return virus_list;
}

virus *readVirus(FILE *file)
{
    virus *v = malloc(sizeof(virus));

    fread(&v->SigSize, sizeof(unsigned short), 1, file);
    fread(v->virusName, sizeof(char), 16, file);
    v->sig = malloc(v->SigSize);
    fread(v->sig, sizeof(unsigned char), v->SigSize, file);

    return v;
}
link *load(link *virus_list)
{
    // printf("Virus list not null\n");

    list_free(virus_list);
    virus_list = malloc(sizeof(link));
    virus_list->vir = NULL;
    virus_list->nextVirus = NULL;
    virus *v;
    FILE *fp;

    fp = loadFromFile();

    char four_first_bytes[4];
    fread(four_first_bytes, sizeof(char), 4, fp);

    char readings;
    readings = fgetc(fp);
    while (readings != EOF)
    {
        readings = ungetc(readings, fp);
        v = readVirus(fp);
        list_append(virus_list, v);

        readings = fgetc(fp);
    }

    // free(v);
    fclose(fp);
    return virus_list;
}

link *print(link *virus_list)
{
    list_print(virus_list, stdout);
    return virus_list;
}
void detect_virus(char *buffer, unsigned int size, link *virus_list)
{
    link *list = virus_list;
    int i = 0;
    int ret;
    while (list != NULL)
    {
        i = 0;
        while (i < size)
        {
            ret = memcmp(buffer + i, list->vir->sig, list->vir->SigSize);
            if (ret == 0)
            {
                fprintf(stdout, "Starting location: %d\nVirus name: %s\nSize of signature: %d\n\n",
                        i, list->vir->virusName, list->vir->SigSize);
                break;
            }

            i++;
        }
        list = list->nextVirus;
    }
}

link *detect(link *virus_list)
{

    FILE *fp;
    fp = loadFromFile();

    char *buffer = (calloc(10000, sizeof(char)));

    unsigned int size;

    size = fread(buffer, 1, 10000, fp);

    detect_virus(buffer, size, virus_list);

    free(buffer);
    fclose(fp);
    return virus_list;
}
void kill_virus(char *fileName, int signitureOffset, int signitureSize)
{
    // fprintf(stdout, "\n\nINSIDE KILL VIRUS - File name: %s\nStarting: %d\nSize: %d\n\n", fileName, signitureOffset, signitureSize);

    FILE *fp;
    fp = fopen(fileName, "r+");
    if (fp == NULL)
    {
        fprintf(stdout, "File doesn't exist\n");
        fclose(fp);
        exit(3);
    }

    fseek(fp, signitureOffset, SEEK_SET);

    char str[signitureSize]; // = 0x90;
    for (int i = 0; i < sizeof(str); i++)
    {
        str[i] = 0x90;
    }
    fwrite(str, 1, signitureSize, fp);

    fclose(fp);
}
link *fix(link *virus_list)
{
    int startingByte;
    int virusSig;
    char *userInput = (char *)(calloc(60, sizeof(char)));
    char *name = (char *)(calloc(18, sizeof(char)));

    fprintf(stdout, "Enter signature file name, starting byte location and the size of the virus signature:\n");
    fgets(userInput, 60, stdin);
    sscanf(userInput, "%s %d %d", name, &startingByte, &virusSig);

    // fprintf(stdout, "\n\nFile name: %s\nStarting: %d\nSize: %d\n\n", name, startingByte, virusSig);

    kill_virus(name, startingByte, virusSig);
    free(name);
    free(userInput);
    return virus_list;
}

link *quit(link *virus_list)
{
    list_free(virus_list);

    exit(3);
    return virus_list;
}
struct fun_desc menu[] = {{"Load Signatures", load},
                          {"Print Signatures", print},
                          {"Detect Viruses", detect},
                          {"Fix File", fix},
                          {"Quit", quit},
                          {NULL, NULL}};

int main(int argc, char **argv)
{

    int bound = sizeof(menu) / sizeof(menu[0]);
    // link *virus_list = (link *)calloc(1, sizeof(link));
    // link *virus_list = malloc(sizeof(link));
    link *virus_list = NULL;
    // virus_list->vir = NULL;
    // virus_list->nextVirus = NULL;
    while (1)
    {
        fprintf(stdout, "\nPlease choose a function:\n");
        for (int i = 1; i < bound; i++)
        {
            fprintf(stdout, "%d) %s\n", i, menu[i - 1].name);
        }
        fprintf(stdout, "Option: ");

        char c = fgetc(stdin);
        int option = atoi(&c);

        if (option >= 0 && option <= bound)
        {
            fprintf(stdout, "\n\nWithin bounds\n");
            virus_list = menu[option - 1].fun(virus_list);
            fprintf(stdout, "\nDONE.\n");
        }
        else
        {
            fprintf(stdout, "\nNot within bounds\n");
            exit(3);
        }
    }
    list_free(virus_list);
    free(virus_list);

    return 0;
}