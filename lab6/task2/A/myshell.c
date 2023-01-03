#include "LineParser.h"
#include <stdlib.h>
#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define SIZE 2048
#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0

typedef struct process{ //coppied from lab6 tasks
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;

//Receive a process list (process_list), a command (cmd), 
//and the process id (pid) of the process running the command. 
void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
    process *newProcess = malloc(sizeof(process));

    

    newProcess->cmd = cmd;
    newProcess->pid = pid;
    newProcess->status = RUNNING;
    newProcess->next = *process_list;

    (*process_list) = newProcess;

    printf("\nadded process: %d\n", (*process_list)->pid);
}

//print the processes.
void printProcessList(process** process_list){
    process *nextProc = *process_list;
    printf("PID\tCOMMAND\tSTATUS\n");
    while (nextProc != NULL)
    {
        printf("while");
        char status[11];
        switch (nextProc->status)
        {
        case TERMINATED:
            strcpy(status, "TERMINATED");
            break;
        case RUNNING:
            strcpy(status, "RUNNING");
            break;
        case SUSPENDED:
            strcpy(status, "SUSPENDED");
            break;
        }

        printf("%d\t%s\t%s\n", nextProc->pid, nextProc->cmd->arguments[0], status);
        nextProc = nextProc->next;
    }
    
}  

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
    
    process ** process_list = malloc(sizeof(process*));
    *process_list = NULL;


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
        //chek proc
        else if(strncmp(parsed->arguments[0], "procs", 5) == 0){
            printProcessList(process_list);
        }
        else{
            //fork
            if(!(pid=fork())) {         //print order messed up first displaying wd then executing command
                if(dFlag == 1){
                    fprintf(stderr, "pid: %d\nExecuting command: %s\n\n", pid, parsed->arguments[0]);
                }
                execute(parsed);
            }
            addProcess(process_list, parsed, pid);
            //wait
            if(parsed->blocking == 1){
                int status;
                waitpid(pid, &status, 0);
            }
        }

    }

    return 0;    
}