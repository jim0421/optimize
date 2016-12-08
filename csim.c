/*************************
NAME=Ke Chen
ID=15302010028
**************************/

#include "cachelab.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>

#define MaxNum 999999999
#define shift(a,b,x) (x >> (a)) & ((1 << (b)) - 1) //get bits from address, x address
												   // a offset; b length

struct data_t
{
	int flag;
	unsigned long long index;
	int timeline;
};

char inst[10],mixture[10];
unsigned long long addr;
int size,s,E,b,maxT,hits,misses,evicts,showflag;
struct data_t **data;


void split()
{
	size = 0;
	addr = 0;
	int len = strlen(mixture);
	int pos = 0;
	while(mixture[pos] != ',')
	{
		int j = 0;
		if(mixture[pos] >= 'a') j = mixture[pos] - 'a' + 10;
		else j = mixture[pos] - '0';
		addr = addr * 16 + j;
		++pos; 
	}
	++pos;
	while(pos < len)
	{
		size = size * 10 + (mixture[pos] - '0');
		++pos;
	}
}

int find(int set,unsigned long long index)
{
	int i;
	for(i = 0;i < E;i++)
	{
		if(data[set][i].flag && data[set][i].index == index) 
		{
			data[set][i].timeline = maxT;
			return 1;
		}
	}
	return 0;
}

int update(int set,unsigned long long index)
{
	int i,mina = MaxNum,pos = 0;
	for(i = 0;i < E;i++)
	{
		if(data[set][i].flag == 0)
		{
			data[set][i].timeline = maxT;
			data[set][i].index = index;
			data[set][i].flag = 1;
			return 0;
		}
	}
	for(i = 0;i < E;i++)
	{
		if(data[set][i].timeline < mina)
		{
			mina = data[set][i].timeline;
			pos = i;
		}
	}
	i = pos;
	data[set][i].timeline = maxT;
	data[set][i].index = index;
	data[set][i].flag = 1;
	return 1;	
}

int S2T(char c[])
{
	int len = strlen(c);
	int re = 0;
	int i;
	for(i = 0;i < len;i++)
		re = re * 10 + c[i] - '0';
	return re;
}


int main(int argc,char* argv[])
{
	int sh,i;
	while((sh = getopt(argc,argv,"vhs:E:b:t:")) != -1)
	{
		switch(sh)
		{
			case 'v': showflag = 1;break;
			case 's': s = S2T(optarg);break;
			case 'E': E = S2T(optarg);break;
			case 'b': b = S2T(optarg);break;
			case 't': freopen(optarg,"r",stdin);break;
		}
	}
	int row = 1 << s,col = E;
	data = (struct data_t **) malloc(sizeof(struct data_t *) * (row + 1));
	for(i = 0;i <= row;i++) data[i] = (struct data_t *) malloc(sizeof(struct data_t) * col);
	while(scanf("%s %s",inst,mixture) != -1) 
	{
		++maxT;
		if(inst[0] == 'I') continue;
		else
		{
			split();
			int set = shift(b,s,addr);//getSet(addr);
			unsigned long long index = shift(b + s,64 - b - s,addr); //getIndex(addr);
			if(find(set,index))
			{
				//hits++;
				if(inst[0] == 'M') 
				{
					printf("%c %llx,%d hit hit\n",inst[0],addr,size);
					hits++;
				}
				else	printf("%c %llx,%d hit\n",inst[0],addr,size);
				hits++;
			}
			else
			{
				misses++;
				printf("%c %llx,%d miss",inst[0],addr,size);
				if(update(set,index))
				{
					if(inst[0] == 'M') 
					{
						printf(" eviction hit");
						++hits;
					}
					else printf(" eviction");
					evicts++;
				}
				else
				{
					if(inst[0] == 'M')
					{
						printf(" hit");
						++hits;
					}
					
				}
				printf("\n");
			}
		}
	}
	printSummary(hits,misses,evicts);
	fclose(stdin);
    	return 0;
}
