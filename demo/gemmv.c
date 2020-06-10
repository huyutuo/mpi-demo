#include "myhead.h"

void gemmv(int m,int n,float *a,int lda,float *x, float *y){
	int i,j;

	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			y[i]+=a[i*lda+j]*x[j];
		}
	}

	return ;
}
