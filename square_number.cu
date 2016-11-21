/*
 * =====================================================================================
 *
 *       Filename:  square_number.cu
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年09月30日 13时33分00秒
 *       Revision:  none
 *       Compiler:  nvcc
 *
 *         Author:  george, 
 *   Organization:  
 *
 * =====================================================================================
 */
#include<stdio.h>

__global__ void fun(int * d_out,int *d_in){
		int idx = threadIdx.x;
		int num = d_in[idx];
		d_out[idx] = num*num*num;
}
int main(){
		const int ARRAY_SIZE = 96;
		const size_t  ARRAY_BYTES = ARRAY_SIZE* sizeof(int);
    		int h_in[ARRAY_SIZE];
		int h_out[ARRAY_SIZE];

		for(int i=0;i<ARRAY_SIZE;i++){
			h_in[i] = i;
		}
		int *d_in;
		int *d_out;
		cudaMalloc(&d_in,ARRAY_BYTES);
		cudaMalloc((void **)&d_out,ARRAY_BYTES);
		
		cudaMemcpy(d_in,h_in,ARRAY_BYTES,cudaMemcpyHostToDevice);
		fun<<<1,ARRAY_SIZE>>>(d_out,d_in);
		cudaMemcpy(h_out,d_out,ARRAY_BYTES,cudaMemcpyDeviceToHost);
	
		printf("Output:\n");
		for(int i=0;i<ARRAY_SIZE;i++){
			printf("%d ",h_out[i]);
		}
		printf("\n");

		cudaFree(d_in);
		cudaFree(d_out);
}
