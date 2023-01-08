#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <elf.h>
#include <fcntl.h>
#include <stdio.h>


void print_phdr(Elf32_Phdr * phdr, int i){
    printf("Program header number %d at address %x\n", i, phdr->p_vaddr);
}

int foreach_phdr(void *map_start, void (*func)(Elf32_Phdr *,int), int arg){
    Elf32_Ehdr * header = (Elf32_Ehdr *) map_start;
    Elf32_Phdr * phdr = (Elf32_Phdr *)(map_start + header->e_phoff);
    int i;
    for(i = 0; i < header->e_phnum; i++){
        func(phdr + i, i);
    }

    return 0;
}

int main(int argc, char** argv){

    char* file_name = argv[1];
    struct stat st;
    void * map = NULL;
    int currentfd = open(file_name, O_RDWR, 0677);

    if (fstat(currentfd, &st) == -1)
        {
            currentfd = -1;
            perror("fstat");
        }
        map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, currentfd, 0);

        if (map == MAP_FAILED)
        {
            printf("mapping failed\n");
        }



    foreach_phdr(map, print_phdr, 5);

    return 0;
}

