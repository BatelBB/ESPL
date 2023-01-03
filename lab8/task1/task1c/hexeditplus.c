#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hexeditplus.h"
#include "units.c"

#define BUF_SIZE 10000


int main(int argc, char **argv)
{
	struct fun_desc menu[] = {
        {"Toggle Debug Mode", toggleDebugMode},
        {"Set File Name", setFileName},
        {"Set Unit Size", setUnitSize},
        {"Load Into Memory", loadIntoMemory},
		{"Toggle Display Mode", toggleDisplayMode},
        {"Memory Display", memoryDisplay},
        {"Save Into File", saveIntoFile},
        {"Memory Modify", memoryModify},
        {"Quit", quit},
        {NULL, NULL}
    };
		
    int upperBound = (sizeof(menu) / sizeof(struct fun_desc));
    state* s = (state*)calloc(sizeof(state), 1);

    while(1){
        //print menu
        checkDebug(s);
        printf("Choose action:\n");
        for(int i = 0; i < upperBound - 1; i++)
            printf("%d-%s\n", i, menu[i].name);

        int action = getInput();
        printf("\n");

        menu[action].fun(s);
        
    }
}

int getInput(){
    while(1){
        char c = fgetc(stdin);
        if(c != '\n'){
            int in = atoi(&c);
            return in;
        }
            
    }
}

void checkDebug(state *s){
    if(s->debug_mode){
        fprintf(stdout, "Debug: unit size = %d    file name = %s    mem count = %d\n", 
                s->unit_size, s->file_name, s->mem_count);
    }
}

void toggleDebugMode(state *s)
{
    if(s->debug_mode){
        fprintf(stderr, "Debug Mode is now off");
        s->debug_mode = 0;
    }
    else{
        fprintf(stderr, "Debug Mode is now on");
        s->debug_mode = 1;
    }

}

void setFileName(state *s)
{
    printf("insert new file name:\n");
    // int done = 0;
    char newFileName[100];  

    // while(!done){
    //     fgets(newFileName, 100, stdin);
    //     if(strcmp(newFileName, "\n") != 0)
    //         done = 1;
    // }
    getSInput(100, newFileName);

    strcpy(s->file_name, newFileName);

	if(s->debug_mode)
        fprintf(stderr, "Debug: file name set to %s \n", s->file_name);
}

void setUnitSize(state *s)
{
	printf("pick a number: ");
    int number = getInput();

    if(number != 1 && number != 2 && number != 4)
        fprintf(stderr, "invalid size\n");
    else{
        if(s->debug_mode)
            fprintf(stderr, "Debug: set size to %d\n", number);

        s->unit_size = number;
    }
}

void getSInput(int size, char* ans){
    char input[size];
    char fname[size];
    while (1)
    {
        fgets(input, size, stdin);
        if(strcmp(input, "\n") != 0){
            sscanf(input, "%s", fname);
            
            strcpy(ans, fname);
            return;
        }
    }
    
}

void getWithFgetsSInput(int size, char* ans){
    char input[size];
    while (1)
    {
        fgets(input, size, stdin);
        if(strcmp(input, "\n") != 0){
            
            strcpy(ans, input);
            return;
        }
    }
    
}

void loadIntoMemory(state *s) 
{
    //check if fname is empty
    if(strcmp(s->file_name, "") == 0){
        fprintf(stderr, "file name is empty\n");
        return;
    }

    //openfile for reading
    FILE* f_p = fopen(s->file_name, "r");
    if (f_p == NULL){
        fprintf(stderr, "cant open file\n");
        return;
    }

    //
    printf("Please enter <location> <length>\n");
    
    char input[15]; 
    getWithFgetsSInput(15, input);

    int location;
    int length;

    sscanf(input, "%x %d", &location,  &length);

    if(s->debug_mode)
        fprintf(stderr, "file name: %s   location: %x    length: %d\n", 
                s->file_name, location, length);

    fseek(f_p, location, SEEK_SET); 
    fread(s->mem_buf, s->unit_size, length, f_p);



    fclose(f_p);

    s->mem_count = length*s->unit_size;

    if(s->debug_mode){
        for(int i = 0; i < s->mem_count; i+=2){
            printf("%#hhx\n", *(s->mem_buf + i));
        }
    }



    printf("Loaded %d units into memory\n", length);


}

void toggleDisplayMode(state *s) 
{
	if(s->display_mode){
        printf("Display flag now off, decimal representation\n");
        s->display_mode = 0;
    }else{
        printf("Display flag now on, hexadecimal representation\n");
        s->display_mode = 1;
    }
}

void memoryDisplay(state *s) 
{
    char input[100];
    printf("ente units num and address: \n");

    getWithFgetsSInput(100, input);

    int u;
    int addr;
    sscanf(input, "%d %x", &u, &addr);

    print_units(stdout, s->mem_buf, u, s->display_mode, s->unit_size);        //doesnt work properly

}

void saveIntoFile(state *s) 
{
	printf("Not implemented yet\n"); 
}

void memoryModify(state *s) 
{
	printf("Not implemented yet\n"); 
}

void quit(state *s)
{
    if(s->debug_mode){
        fprintf(stderr, "Debug: quitting\n");
    }
	exit(0);
}