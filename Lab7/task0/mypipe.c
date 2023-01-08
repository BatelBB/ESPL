#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define STDOUT 1

int main(int argc, char** argv){
    int pipefd[2];
    int pid;
    char* msg = "hello";
    char buf;

    if(pipe(pipefd) == -1){
        perror("pipe");
        exit(1);
    }

    if(!(pid=fork())){
        //child
        close(pipefd[0]);   //close unused end of pipe

        write(pipefd[1], msg, strlen(msg));

        close(pipefd[1]);
        exit(0);
        
    }
    else{
        //parent
        close(pipefd[1]);   //close unused end of pipe        

        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT, &buf, 1);

        close(pipefd[0]);

        exit(0);
    }

    
}