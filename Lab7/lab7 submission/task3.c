#include "LineParser.h"
#include <stdlib.h>
#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

#define SIZE 2048
#define TERMINATED -1
#define RUNNING 1
#define SUSPENDED 0


typedef struct process{ //coppied from lab6 tasks
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;

void freeProcessList(process* process_list);
void freeProcess(process* p);
void updateProcessList(process **process_list);
void updateProcessStatus(process* process_list, int pid, int status);
void addProcess(process** process_list, cmdLine* cmd, pid_t pid);
void printProcessList(process** process_list);
void cleanList(process** process_list);
void removeProcess(process** process_list, int pid);
void execute(cmdLine *pCmdLine);




//free all memory allocated for the process list.
void freeProcessList(process* process_list){
    if(process_list != NULL){
        freeProcessList(process_list->next);
        freeProcess(process_list);
    }
}

void freeProcess(process* p){
    // printf("hello pid: %d\n", p->pid);
    freeCmdLines(p->cmd);
    free(p);
}

void updateProcessList(process **process_list){
    process* cur = *process_list;
    while(cur != NULL){
        int status = 0;
        int result = waitpid(cur->pid, &status, WNOHANG | WUNTRACED | WCONTINUED);
        printf("\nresult: %d\t status: %d\n", result, status);
        if(result>0){
            updateProcessStatus(cur, cur->pid, status);
        }
        else if(result == -1){
            cur->status = TERMINATED;
        }


        cur = cur->next;
    }
}

void updateProcessStatus(process* process_list, int pid, int status){
    process* cur = process_list;
    if(WIFSTOPPED(status)){
        cur->status = SUSPENDED;
    }
    else if(WIFCONTINUED(status)){
        cur->status = RUNNING;
    }
    else if(WIFSIGNALED(status) || WIFEXITED(status)){
        printf("whats is this?? %d\npid: %d\n", status, cur->pid);
        cur->status = TERMINATED;
    }
    
}


//Receive a process list (process_list), a command (cmd), 
//and the process id (pid) of the process running the command. 
void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
    process *newProcess = malloc(sizeof(process));

    newProcess->cmd = cmd;
    newProcess->pid = pid;
    newProcess->status = RUNNING;
    newProcess->next = *process_list;

    (*process_list) = newProcess;

}

//print the processes.
void printProcessList(process** process_list){
    updateProcessList(process_list);

    process *nextProc = *process_list;
    printf("PID\tCOMMAND\tSTATUS\n");
    while (nextProc != NULL)
    {
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

    cleanList(process_list);
    
}

void cleanList(process** process_list){
    process* cur = *process_list;
    process* cur2 = *process_list;
    while (cur != NULL)
    {
        cur2 = cur->next;
        if(cur->status == TERMINATED){
            removeProcess(process_list, cur->pid);
        }
        cur = cur2;
    }
    
}

void removeProcess(process** process_list, int pid){
    process* prev = NULL;
    process* post = NULL;
    process* toRemove = NULL;

    process * cur = *process_list;
    int found = 0;
    while(cur != NULL && found == 0){
        if(cur->pid == pid){
            printf("here: %d\n", cur->pid);
            toRemove = cur;
            post = cur->next;
            found = 1;
        }
        else{
            prev = cur;
            cur = cur->next;
        }
    }
    toRemove->next=NULL;
    if(prev == NULL){
        
        *process_list = post;
    }
    else{
        prev->next = post;
    }
    freeProcess(toRemove);

}

void execute(cmdLine *pCmdLine){
    execvp(pCmdLine->arguments[0], pCmdLine->arguments);
    perror("ERROR");
    freeCmdLines(pCmdLine);
    _exit(1);
}

 void sendSignal(char* signame, int pid){
        int res;
        if(strncmp(signame, "suspend", 6) == 0){
            res = kill(pid, SIGTSTP);
        }
        else if(strncmp(signame, "kill", 4) == 0){
            printf("pid: %d\n", pid);
            res = kill(pid, SIGINT);
        }
        else if(strncmp(signame, "wake", 4) == 0){
            res = kill(pid, SIGCONT);
        }

        if(res < 0){
            perror("ERROR");
        }
        else{
            printf("signal sent");
        }
    }


void redirectInOut(cmdLine* parsed){
    int fd;
    if (parsed->inputRedirect)
    {
        close(STDIN_FILENO);
        fd = open(parsed->inputRedirect, O_RDONLY, S_IRUSR);
        dup2(fd, 0); //replace stdin with redirect
    }
    if (parsed->outputRedirect)
    {
        close(STDOUT_FILENO);
        fd = open(parsed->outputRedirect, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dup2(fd, 1); //replace stdout with redirect
    }
}

void myPipeLine(cmdLine* parsed, cmdLine* second){
    
    int pid;
    int pid2;
    int pipefn[2];

    //step1
    int p_res = pipe(pipefn);
    if (p_res == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    //step2
    if(!(pid=fork())) {  
        //step 3.1
        close(STDOUT_FILENO);
        //step 3.2
        dup(pipefn[1]);
        //step 3.3
        close(pipefn[1]);

        redirectInOut(parsed);


        
        //step 3.4
        execvp(parsed->arguments[0], parsed->arguments);
        _exit(1);

    }
    else{
        // step 8
        waitpid(pid, NULL, 0);

        //step 4
        close(pipefn[1]);

        //step 5
        if(!(pid2=fork())){

            //step 6.1
            close(STDIN_FILENO);
            //step 6.2
            dup(pipefn[0]);
            //step 6.3
            close(pipefn[0]);

            redirectInOut(second);

            //step 6.4
            execvp(second->arguments[0], second->arguments);
            _exit(1);
        }
        else{
            //step 8
            waitpid(pid2, NULL, 0);
            //step 7
            close(pipefn[0]);
            
        }
    }
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
        

        cmdLine* second = parsed->next;

        
        if(strncmp(parsed->arguments[0], "quit", 4) == 0){
            freeCmdLines(parsed);
            freeProcessList(*process_list);
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
        else if(strncmp(parsed->arguments[0], "suspend", 6) == 0){
            sendSignal("suspend", atoi(parsed->arguments[1]));
        }
        else if(strncmp(parsed->arguments[0], "kill", 4) == 0){
            sendSignal("kill", atoi(parsed->arguments[1]));
        }
        else if(strncmp(parsed->arguments[0], "wake", 4) == 0){
            sendSignal("wake", atoi(parsed->arguments[1]));
        }
        else if(second != NULL){
            myPipeLine(parsed, second);
        }
        else{
            //fork
            if(!(pid=fork())) {         
                if(dFlag == 1){
                    fprintf(stderr, "pid: %d\nExecuting command: %s\n\n", pid, parsed->arguments[0]);
                }

                redirectInOut(parsed);
                execute(parsed);
            }
            else{
                addProcess(process_list, parsed, pid);

                //wait
                if(parsed->blocking == 1){
                    int status;
                    waitpid(pid, &status, 0);
                }
            }
            
        }
    
    }

    return 0;    
}