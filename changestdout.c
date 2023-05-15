#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>



int main()
{
  int x = fork();
  if(x == 0)
    {
      close(STDOUT_FILENO);
      int fd = open("test.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
      // printf();
      dprintf(fd, "%d\n",fd);
      // printf("lol\n");
      char *myargs[3];
      myargs[0] = "ls\0";
      myargs[1] = "/home/poorna";
      myargs[2] = NULL;
      execvp(myargs[0], myargs);
    }
  else
    {
      wait(NULL);
      return 0;
    }
}
