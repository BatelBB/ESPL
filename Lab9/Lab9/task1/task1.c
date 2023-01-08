#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <elf.h>
#include <fcntl.h>

struct fun_desc
{
    char *name;
    void (*fun)();
};
int getAction();
void toggleDebugMode();
void examineElfFile();
void printProgramHeaderStats();
void printSectionHeaderStats();
void printFIleOffset();
void printEntryPoint();
void printEncodingScheme();
void checkMagicNumbers();
void printMagicNumbers();
void loadFileToMap(char *);
void closeFile();
void printSectionNames();
char *getSectionHeaderType(int num);
void printSymbols();
void RelocateTables();
void quit();

int debug = 0;
char *fName = NULL;
int currentfd = -1;
struct stat st;
void *map = NULL;
Elf32_Ehdr *header = NULL;
Elf32_Shdr *section_hdr = NULL;

int main(int argc, char **argv)
{
    map = calloc(sizeof(void *), 1);

    struct fun_desc menu[] = {
        {"Toggle Debug Mode", toggleDebugMode},
        {"Examine ELF File", examineElfFile},
        {"Print Section Names", printSectionNames},
        {"Print Symbols", printSymbols},
        {"Relocation Tables", RelocateTables},
        {"Quit", quit},
        {NULL, NULL}};

    while (1)
    {

        printf("Choose action:\n");
        int i;
        for (i = 0; i < 6; i++)
            printf("%d-%s\n", i, menu[i].name);

        int choose;
        char input_line[100];
        fgets(input_line, 100, stdin);
        sscanf(input_line, "%d", &choose);
        /* int action = getAction();*/

        menu[choose].fun();
    }
}

int getAction()
{
    while (1)
    {
        char c = fgetc(stdin);
        int i;
        if (c != '\n')
        {
            i = atoi(&c);
            return i;
        }
    }
}

void toggleDebugMode()
{
    if (debug == 0)
    {
        debug = 1;
        printf("Debug mode is now on\n");
    }
    else
    {
        debug = 0;
        printf("Debug mode is now off\n");
    }
}

void examineElfFile()
{
    printf("insert file name: \n");
    char buff[100];
    char fName[100];
    fgets(buff, 100, stdin);
    sscanf(buff, "%s", fName);
    /*printf("\nfile name is: %s\n", fName);*/

    closeFile();

    currentfd = open(fName, O_RDWR, 0677);
    /*printf("\ncurrentfd is: %d\n", currentfd);*/
    if (currentfd == -1)
    {
        perror("Error - could not open file");
        return;
    }
    else
    {
        if (fstat(currentfd, &st) == -1)
        {
            close(currentfd);
            currentfd = -1;
            perror("fstat");
            exit(1);
        }
        map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, currentfd, 0);
        section_hdr = (Elf32_Shdr *)(map + header->e_shoff);

        if (map == MAP_FAILED)
        {
            printf("mapping failed\n");
            closeFile();
        }

        header = (Elf32_Ehdr *)map;
    }

    printMagicNumbers();

    checkMagicNumbers();

    printEncodingScheme();

    printEntryPoint();

    printFIleOffset();

    printSectionHeaderStats();

    printProgramHeaderStats();
}

void printProgramHeaderStats()
{
    printf("program header table offset: %d\n", header->e_phoff);
    printf("number of program headers: %d\n", header->e_phnum);
    printf("size of each program header: %d\n", header->e_phentsize);
}

void printSectionHeaderStats()
{
    printf("number of section headers: %d\n", header->e_shnum);
    printf("size of each section header: %d\n", header->e_shentsize);
}

void printFIleOffset()
{
    printf("file offset: %d\n", header->e_shoff);
}

void printEntryPoint()
{
    printf("entry point: %x\n", header->e_entry);
}

void printEncodingScheme()
{
    printf("data encoding scheme of the object file: ");
    int i = header->e_ident[EI_DATA];

    if (i == 1)
    {
        printf("2s compliment little endian\n");
    }
    else if (i == 2)
    {
        printf("2s compliment big endian\n");
    }
    else
    {
        perror("invalid encoding scheme\n");
    }
}

void checkMagicNumbers()
{
    if (*((char *)map) == 0x7F && *((char *)map + 1) == 'E' &&
        *((char *)map + 2) == 'L' && *((char *)map + 3) == 'F')
    {
        printf("correct magic numbers\n");
    }
    else
    {
        printf("not elf file magic numbers\n");
        closeFile();
    }
}

void printMagicNumbers()
{
    printf("magic numbers: %c%c%c\n", header->e_ident[EI_MAG1], header->e_ident[EI_MAG2], header->e_ident[EI_MAG3]);
}
/*
void loadFileToMap(char *fname)
{
    currentfd = open(fName, O_RDWR, 0677);
    printf("\ncurrentfd is: %d\n", currentfd);
    if (currentfd == -1)
    {
        perror("Error - could not open file");
        return;
    }
    else
    {
        map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, currentfd, 0);

        if (map == MAP_FAILED)
        {
            printf("mapping failed\n");
            unmapCUrrentFile();
        }

        header = (Elf32_Ehdr *)map;
    }
}
*/
void CloseFile()
{
    if (currentfd != -1)
    {
        close(currentfd);
    }
}

void printSectionNames()
{
    printf("[index] section_name section_address section_offset section_size  section_type");
    if (debug)
        printf("  offset\n");
    else
    {
        printf("\n");
    }

    /*getting names table:*/
    char *shstrtab = (char *)(map + section_hdr[header->e_shstrndx].sh_offset);

    int i;
    for (i = 0; i < header->e_shnum; i++)
    {
        char *name = shstrtab + section_hdr[i].sh_name;
        char *sectionType = getSectionHeaderType(section_hdr[i].sh_type);
        printf("[%d]\t %-17s%08X\t %06X\t\t %d\t  %s", i, name, section_hdr[i].sh_addr, section_hdr[i].sh_offset,
               section_hdr[i].sh_size, sectionType);
        if (debug)
        {
            printf("\t%d\n", section_hdr[header->e_shstrndx].sh_offset + section_hdr[i].sh_offset);
        }
        else
        {
            printf("\n");
        }
    }
}

char *getSectionHeaderType(int num)
{
    switch (num)
    {
    case 0:
        return "SHT_NULL";
    case 1:
        return "SHT_PROGBITS";
    case 2:
        return "SHT_SYMTAB";
    case 3:
        return "SHT_STRTAB";
    case 4:
        return "SHT_RELA";
    case 5:
        return "SHT_HASH";
    case 6:
        return "SHT_DYNAMIC";
    case 7:
        return "SHT_NOTE";
    case 8:
        return "SHT_NOBITS";
    case 9:
        return "SHT_REL";
    case 10:
        return "SHT_SHLIB";
    case 11:
        return "SHT_DYNSYM";
    default:
        return "";
    }
}

void printSymbols()
{
    printf("Not Implemented Yet\n");
}

void RelocateTables()
{
    printf("Not Implemented Yet\n");
}

void quit()
{
    if (munmap(map, (size_t)st.st_size) < 0)
    {
        perror("munmap failed");
    }
    close(currentfd);
    exit(0);
}