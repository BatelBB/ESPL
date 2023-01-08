#include <stdlib.h>
#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

#define SIZE 2048




// void execute(cmdLine *pCmdLine){
//     execvp(pCmdLine->arguments[0], pCmdLine->arguments);
//     perror("ERROR");
//     freeCmdLines(pCmdLine);
//     _exit(1);
// }


int main(int argc, char** argv){

    int pid;
    int pid2;
    int dFlag = 0;
    char* cmd1[] = {"ls","-l", NULL};
    char* cmd2[] = {"tail","-n", "2", NULL};
    int pipefn[2];


    for(int i =0; i<argc && dFlag == 0; i++){
        if(strncmp(argv[i], "-d", 2) == 0)
            dFlag = 1;
    }
    

    //step1
    int p_res = pipe(pipefn);
    if (p_res == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if(dFlag == 1){
        fprintf(stderr, "(parent_process>forking…)\n");
    }
    //step2
    if(!(pid=fork())) {  
        if(dFlag == 1){
            fprintf(stderr, "(child1>redirecting stdout to the write end of the pipe…)\n");
        }
        //step 3.1
        close(STDOUT_FILENO);
        //step 3.2
        dup(pipefn[1]);
        //step 3.3
        close(pipefn[1]);

        if(dFlag == 1){
            fprintf(stderr, "(child1>going to execute cmd: …)\n");
        }
        //step 3.4
        execvp(cmd1[0], cmd1);
        _exit(1);

    }
    else{
        if(dFlag == 1){
            fprintf(stderr, "(parent_process>created process with id: %d)\n", pid);
        }
        if(dFlag == 1){
            fprintf(stderr, "(parent_process>waiting for child processes to terminate…)\n");
        }
        // step 8
        waitpid(pid, NULL, 0);

        if(dFlag == 1){
            fprintf(stderr, "(parent_process>closing the write end of the pipe…)\n");
        }
        //step 4
        close(pipefn[1]);

        //step 5
        if(!(pid2=fork())){
            if(dFlag == 1){
                fprintf(stderr, "(c(child2>redirecting stdin to the read end of the pipe…)\n");
            }
            //step 6.1
            close(STDIN_FILENO);
            //step 6.2
            dup(pipefn[0]);
            //step 6.3
            close(pipefn[0]);

            if(dFlag == 1){
                fprintf(stderr, "(child2>going to execute cmd: …)\n");
            }
            //step 6.4
            execvp(cmd2[0], cmd2);
            _exit(1);
        }
        else{
            if(dFlag == 1){
                fprintf(stderr, "(parent_process>waiting for child processes to terminate…)\n");
            }
            //step 8
            waitpid(pid2, NULL, 0);

            if(dFlag == 1){
                fprintf(stderr, "(parent_process>created process with id: %d)\n", pid2);
            }
            if(dFlag == 1){
                fprintf(stderr, "(parent_process>closing the read end of the pipe…)\n");
            }
            //step 7
            close(pipefn[0]);
            
        }
        
        if(dFlag == 1){
            fprintf(stderr, "(parent_process>exiting…)\n");
        }
        _exit(0);
    }

    
    return 0;    
}