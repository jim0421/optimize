typedef struct {
	int argb[4]; /* Alpha Red Green Blue */
} pixel_t;

typedef struct {
	int len;
	pixel_t *data;
} pixels_t;

static pixels_t pixels;

static pixel_t minAlpha;

int get_length(pixels_t *p) { return p->len; }

pixel_t *get_pixel_at(int index) {
	return pixels.data + index;
}


/*#define ORIGINAL*/
/*#define OPTIMIZE_ONE*/
#define OPTIMIZE_TWO

#ifdef ORIGINAL
void dosth_and_find(pixels_t *p, pixel_t *minAlpha) {
	int i, j;
	minAlpha = get_pixel_at(0);
	for (i = 1; i < 4; i++) {
		for (j = 0; j < get_length(p); j++) {
			get_pixel_at(j)->argb[i] = get_pixel_at(j)->argb[i]*2%255;
			if (get_pixel_at(j)->argb[0] < minAlpha->argb[0])
				minAlpha = get_pixel_at(j);
		}
	}
}
#endif

#ifdef OPTIMIZE_ONE
#define K 2 // (number of ways)
void dosth_and_find(pixels_t *p, pixel_t *mintrans)
{
    int i, j;
    /* Elimminating Loop Inefficiencies */
    int length = get_length(p);
    int limit = length - K;
    /*Used record index of mintrans pixel(Reducing Unneeded Reference) */
    int minIndex1, minIndex2 = 0;
    int minAlpha1, minAlpha2 = 255;

    /* Used to reduce procedure call */
    pixel_t *cur1, *cur2;
    for ( i = 1; i < 4; i++  ) {
        for ( j = 0; j < limit; j+=2  ) {

            cur1 = get_pixel_at(j);
            cur2 = get_pixel_at(j+1);
            /* Unrolling & Parallelism */
            cur1->argb[i] = cur1->argb[i] * 2 % 255;
            cur2->argb[i] = cur2->argb[i] * 2 % 255;

			if ( cur1->argb[0] < minAlpha1 ) {
				minIndex1 = j;
				minAlpha1 = cur1->argb[0];
			}
			if ( cur2->argb[0] < minAlpha2 ) {
				minIndex2 = j+1;
				minAlpha2 = cur2->argb[0];
			}
		}
		for ( ; j < length; j++) {
			cur1 = get_pixel_at(j);
			cur1->argb[i] = cur1->argb[i] * 2 % 255;
			if ( cur1->argb[0] < minAlpha1 ) {
				minIndex1 = j;
				minAlpha1 = cur1->argb[0];
			}
		}
		mintrans = get_pixel_at( minIndex1 < minIndex2 ?
			minIndex1 : minIndex2 );
	}
}
#endif

#ifdef OPTIMIZE_TWO
void dosth_and_find(pixels_t *p, pixel_t *mintrans)
{
	int i;
	/* Elimminating Loop Inefficiencies */
	int length = get_length(p) * 4;
	/* Used record index of mintrans pixel(Reducing Unneeded
	Reference) */
	int minIndex = 0;
	int minAlpha = 255;
	/* treat data as one-dimension array */
	int *data = (int *)get_pixel_at(0);

	/* length is multiple of 4, don’t need limit */
	for ( i = 0; i < length; i+=4 ) {
		data[i+1] = data[i+1] * 2 % 255;
		data[i+2] = data[i+2] * 2 % 255;
		data[i+3] = data[i+3] * 2 % 255;
		if ( data[i] < minAlpha ) {
			minIndex = i/4;
			minAlpha = data[i];
		}
	}
	mintrans = get_pixel_at(minIndex);
}
#endif





#ifndef DEBUG
#define DEBUG
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#define PIXEL_NUM 1920*1080*40
int main(){
    pixel_t *pixelArray = (pixel_t*)malloc(sizeof(pixel_t) * PIXEL_NUM);
    fprintf(stdout, "$1 is %ld and $2 is %ld\n", sizeof(pixel_t), sizeof(pixels));
    /*int *4vals = NULL;*/
    int i = 0;
    for (;i < PIXEL_NUM; i++){
        /*pixelArray[i].argb = {rand()%256,rand()%256,rand()%256,rand()%256};*/
        pixelArray[i].argb[0] = rand()%256;
        pixelArray[i].argb[1] = rand()%256;
        pixelArray[i].argb[2] = rand()%256;
        pixelArray[i].argb[3] = rand()%256;
    }
    fprintf(stdout, "finish random assignment\n");
    pixels.len=PIXEL_NUM;
    pixels.data=pixelArray;
    
    struct timeval tv_begin, tv_end;
    gettimeofday(&tv_begin,NULL);
    dosth_and_find(&pixels, &minAlpha);
    gettimeofday(&tv_end,NULL);
    double timeDiff = (tv_end.tv_sec-tv_begin.tv_sec) +\
    (tv_end.tv_usec-tv_begin.tv_usec)/1000000.0;
    fprintf(stdout, "time diff is %lf\n", timeDiff);
    fprintf(stdout, "minAlpha is %d\n", minAlpha.argb[0]);

	return 0;
}

#endif
