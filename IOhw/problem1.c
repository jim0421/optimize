#include "csapp.h"
#include <unistd.h>
int main(){
int fd1, fd2;
char c;
fd1 = open("foobar.txt", O_RDONLY, 0);
fd2 = open("foobar.txt", O_RDWR, 0);
if (fork() == 0){
dup2(fd1, fd2);
 read(fd1, &c, 1);//Position A
 read(fd2, &c, 1);
}
int ret = wait(NULL);
read(fd1, &c, 1);
printf("character read from fd1 : %c\n", c);
read(fd2, &c, 1);
printf("character read from fd2 : %c\n", c);
return 0;
}
