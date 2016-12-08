/*
 * =====================================================================================
 *
 *       Filename:  notebook.c
 *
 *    Description:  write note in one line
 *
 *        Version:  1.0
 *        Created:  06/20/2016 12:00:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  George Fan, 
 *   Organization:  fuppi
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define currentNB "fsh_Notebook"

int main(int argc, char *argv[])
{
    FILE *fp = fopen(currentNB,"a+");
	char *resultStr = (char*)malloc(sizeof(char)*512);
	
	/* record time */
    //system("Date >> fsh_Notebook");
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
    sprintf(resultStr, "<%s>", strtok(asctime(tm),"\n")); 
	fprintf(fp, "%s ",resultStr);
    
	// <improve> with -t tag option 
    /* record args */
	int i;
    for (i = 1; i < argc; i++)
    {
        fprintf(fp, "%s ", argv[i]);
    }
    fprintf(fp, "\n");
    fclose(fp);
}
