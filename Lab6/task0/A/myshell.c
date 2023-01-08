#include "LineParser.h"
#include <stdlib.h>
#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define SIZE 2048

void execute(cmdLine *pCmdLine){
    execvp(pCmdLine->arguments[0], pCmdLine->arguments);
    perror("ERROR");
    freeCmdLines(pCmdLine);
    exit(1);
}

int main(int argc, char** argv){

    char buf[PATH_MAX];
    getcwd(buf, PATH_MAX);
    cmdLine* parsed = NULL;

    while (1)
    {

        printf("wd: %s\n", buf);

        char line[SIZE];
        fgets(line, SIZE, stdin);

        parsed = parseCmdLines(line);
        if(strncmp(parsed->arguments[0], "quit", 4) == 0){
            freeCmdLines(parsed);
            exit(0);
        }

        // printf("%s", ((parsed->arguments)+1)[0]);

        execute(parsed);

    }
    



    return 0;    
}