#include<stdio.h>
#include<unistd.h>
int main()
{
  for(;;)
    {
      printf(">>> ");
      char x[20];
      x[0] = '\0';
      scanf("%[^\n]", x);
      //      getchar();
      printf("The string entered is: %s\n", x);
      sleep(1);
    }
}
