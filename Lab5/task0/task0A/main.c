#include "util.h"

#define SYS_WRITE 4
#define STDOUT 1
#define SYS_OPEN 5
#define O_RDWR 2
#define SYS_SEEK 19
#define SEEK_SET 0
#define SHIRA_OFFSET 0x291

extern int system_call();

int main (int argc , char* argv[], char* envp[])
{

  int bytes_count = strlen(argv[0]);
  bytes_count += strlen(argv[1]);
  bytes_count += strlen(argv[2]);


  char msg[] = "hello world";
  system_call(SYS_WRITE, STDOUT, *argv, bytes_count+2);


  return 0;
}
