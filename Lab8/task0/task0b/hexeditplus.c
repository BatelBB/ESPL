#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hexeditplus.h"



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
    int done = 0;
    char newFileName[100];  

    while(!done){
        fgets(newFileName, 100, stdin);
        if(strcmp(newFileName, "\n") != 0)
            done = 1;
    }

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

void loadIntoMemory(state *s) 
{
	printf("Not implemented yet\n"); 
}

void toggleDisplayMode(state *s) 
{
	printf("Not implemented yet\n"); 
}

void memoryDisplay(state *s) 
{
	printf("Not implemented yet\n"); 
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