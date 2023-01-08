#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

Elf32_Ehdr *header = NULL;
Elf32_Phdr *phdr = NULL;
char *string_table = NULL;

char *getpType(Elf32_Word i);
void print_1a(Elf32_Phdr *phdr, int i);
void print_phdr(Elf32_Phdr *phdr, int i);
int foreach_phdr(void *map_start, void (*func)(Elf32_Phdr *, int), int arg);
void load_phdr(Elf32_Phdr *phdr, int i);
int getProtFlags(Elf32_Phdr *phdr);
int startup(int argc, char **argv, void (*start)());

void print_1a(Elf32_Phdr *phdr, int i)
{

    char *type = getpType(phdr->p_type);

    char flags[4] = {' ', ' ', ' ', '\0'};

    if (phdr->p_flags & PF_R)
    {
        flags[0] = 'R';
    }
    if (phdr->p_flags & PF_W)
    {
        flags[1] = 'W';
    }
    if (phdr->p_flags & PF_X)
    {
        flags[2] = 'E';
    }

    printf("%s 0x%x 0x%x 0x%x 0x%x 0x%x %s 0x%x\n", type, phdr->p_offset, phdr->p_vaddr, phdr->p_paddr, phdr->p_filesz, phdr->p_memsz, flags, phdr->p_align);
}

void print_1b(Elf32_Phdr *phdr, int i)
{

    char *type = getpType(phdr->p_type);

    char flags[4] = {' ', ' ', ' ', '\0'};
    int prot_flags = 0;
    int map_flags = MAP_PRIVATE | MAP_FIXED;

    if (phdr->p_flags & PF_R)
    {
        flags[0] = 'R';
        phdr->p_type == PT_LOAD ? prot_flags += PROT_READ : 0;
    }
    if (phdr->p_flags & PF_W)
    {
        flags[1] = 'W';
        phdr->p_type == PT_LOAD ? prot_flags += PROT_WRITE : 0;
    }
    if (phdr->p_flags & PF_X)
    {
        flags[2] = 'E';
        phdr->p_type == PT_LOAD ? prot_flags += PROT_EXEC : 0;
    }

    printf("%s 0x%x 0x%x 0x%x 0x%x 0x%x %s 0x%x\n", type, phdr->p_offset, phdr->p_vaddr, phdr->p_paddr, phdr->p_filesz, phdr->p_memsz, flags, phdr->p_align);
    if (phdr->p_type == PT_LOAD)
        printf("prot:%d   map:%d\n", prot_flags, map_flags);
}

int getProtFlags(Elf32_Phdr *phdr){
    int prot_flags = 0;

    if (phdr->p_flags & PF_R)
        prot_flags += PROT_READ;
    if (phdr->p_flags & PF_W)
        prot_flags += PROT_WRITE;
    if (phdr->p_flags & PF_X)
        prot_flags += PROT_EXEC;
    
    return prot_flags;
}

char *getpType(Elf32_Word i)
{
    switch (i)
    {
    case PT_NULL:
        return "NULL";
    case PT_LOAD:
        return "LOAD";
    case PT_DYNAMIC:
        return "DYNAMIC";
    case PT_INTERP:
        return "INTERP";
    case PT_NOTE:
        return "NOTE";
    case PT_SHLIB:
        return "SHLIB";
    case PT_PHDR:
        return "PHDR";
    case PT_LOPROC:
        return "LOPROC";
    case PT_HIPROC:
        return "HIPROC";
    default:
        return "";
    }
}

void print_phdr(Elf32_Phdr *phdr, int i)
{
    printf("Program header number %d at address %x\n", i, phdr->p_vaddr);
}

void load_phdr(Elf32_Phdr *phdr, int fd){
    if(phdr->p_type == PT_LOAD){
        int vaddr = phdr->p_vaddr&0xfffff000;
        int offset = phdr->p_offset&0xfffff000;
        int padding = phdr->p_vaddr & 0xfff;
        void* map = mmap((void *)vaddr, phdr->p_memsz+padding, getProtFlags(phdr), MAP_PRIVATE | MAP_FIXED, fd, offset); 
        if(map < 0){
            perror("map failed");
            exit(1);
        }
        print_1b(phdr, 0);
    }
    
}

int foreach_phdr(void *map_start, void (*func)(Elf32_Phdr *, int), int arg)
{
    header = (Elf32_Ehdr *)map_start;
    phdr = (Elf32_Phdr *)(map_start + header->e_phoff);

    int i;
    for (i = 0; i < header->e_phnum; i++)
    {
        func(phdr + i, arg);
    }

    return 0;
}

int main(int argc, char **argv)
{

    char *file_name = argv[1];
    struct stat st;
    void *map = NULL;
    int currentfd = open(file_name, O_RDWR, 0677);

    if (fstat(currentfd, &st) == -1)
    {
        currentfd = -1;
        perror("fstat");
    }
    map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, currentfd, 0);
    header = (Elf32_Ehdr *) map;
    if (map == MAP_FAILED)
    {
        printf("mapping failed\n");
    }

    // printf("%x %x %x %x %x \n", MAP_FIXED, MAP_PRIVATE, PROT_READ, PROT_WRITE, PROT_EXEC);

    //printf("Type Offset VirtAddr PhysAddr FileSiz MemSiz Flg Align\n");
    foreach_phdr(map, load_phdr, currentfd);
    startup(argc-1, argv+1, (void *)(header->e_entry));
    return 0;
}
