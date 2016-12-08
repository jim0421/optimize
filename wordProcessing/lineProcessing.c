/*************************************************************************
	> File Name: lineProcessing.c
	> Author: georgefan 
	> Mail: mc_george123@hotmail.com
	> Created Time: 2016年12月08日 星期四 14时22分10秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fshUtil.h"

int main(int argc, char *argv[])
{
    if (argc != 3){
        printf("usage give the filename\n\
                for example\n\
                ./a.out fileIn.type fileOut.type\n");
        return 1;
    }

    String fileIn = newString; 
    sprintf(fileIn, "%s", argv[1]);
    String fileOut = newString; 
    sprintf(fileOut, "%s", argv[2]);
    FILE *fpIn = fopen(fileIn,"r"); //$$checking
    FILE *fpOut = fopen(fileOut, "a+"); //for wrong output 

    char* buffer = newBuffer;
    size_t len;
    ssize_t read;
    char* pos;
    /*http://man7.org/linux/man-pages/man3/getdelim.3.html*/
    while ((read = getline(&buffer, &len, fpIn)) != -1){
        if ((pos=strchr(buffer, '\n')) != NULL)
            *pos = '\0';
        /*fprintf(fpOut,"%s", strtok(buffer,"\n"));*/
        //$$strtok fail, whether it has buffer in c++
        //checking impl
        if (buffer[0] == '[')
            fprintf(fpOut,"\n%s", buffer);
        else
            fprintf(fpOut,"%s", buffer);
    }
    fclose(fpIn);
    fclose(fpOut);
    free(buffer);
    printf("String size is %d\n", STRING_SIZE);
    return 0;
}
