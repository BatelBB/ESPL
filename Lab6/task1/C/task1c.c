#include "LineParser.h"
#include <stdlib.h>
#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define SIZE 2048

void execute(cmdLine *pCmdLine){
    execvp(pCmdLine->arguments[0], pCmdLine->arguments);
    perror("ERROR");
    freeCmdLines(pCmdLine);
    _exit(1);
}

int main(int argc, char** argv){

    char buf[PATH_MAX];
    getcwd(buf, PATH_MAX);
    cmdLine* parsed = NULL;
    int pid;
    int dFlag = 0;
    
    for(int i =0; i<argc && dFlag == 0; i++){
        if(strncmp(argv[i], "-D", 2) == 0)
            dFlag = 1;
    }
    
    while (1)
    {
        //print wd
        printf("wd: %s\n", buf);
        //get input
        char line[SIZE];
        fgets(line, SIZE, stdin);
        //parse input
        parsed = parseCmdLines(line);
        if(strncmp(parsed->arguments[0], "quit", 4) == 0){
            freeCmdLines(parsed);
            _exit(0);
        }
        //chek cd
        if(strncmp(parsed->arguments[0], "cd", 2) == 0){
            int res = chdir(parsed->arguments[1]);
            if (res == -1)
                fprintf(stderr, "cannot cd");
            getcwd(buf, PATH_MAX);
        }
        else{
            //fork
            if(!(pid=fork())) {         //print order messed up first displaying wd then executing command
                if(dFlag == 1){
                    fprintf(stderr, "pid: %d\nExecuting command: %s\n\n", pid, parsed->arguments[0]);
                }
                execute(parsed);
            }
            //wait
            if(parsed->blocking == 1){
                int status;
                waitpid(pid, &status, 0);
            }
        }

    }

    return 0;    
}