#include "csapp.h"
#include "stdlib.h"
#include "stdio.h" 
int main(){
    int fd;
    char *buf;
    buf = (char *)malloc(sizeof(char)*50);
    fd = open("out.txt", O_RDONLY, 0);
    int ret = rio_readn(fd, buf, 20);
    printf("This is ret %d\n", ret);
    return 0;

}
