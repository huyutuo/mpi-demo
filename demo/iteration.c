#include "myhead.h"

void iteration(MPI_Comm comm, int np, int iam,int n, int en, float *a, int lda, float *b, float *x, int num){
	int i,j,*rc;
	float *y;

	rc = (int *)malloc(np*sizeof(int));
	for(i=0; i<np; i++){
		rc[i] = en;
	}
	y = (float *)malloc(n*sizeof(float));
	for(i=0;i<np;i++){
		if(iam == 0){
			for(j=0;j<n;j++) y[j] = b[j];
		}else{
			for(j=0;j<n;j++) y[j] = b[j];
		}
		gemmv(n,en,a,lda,x,y);
		MPI_Reduce_scatter(y,x,rc,MPI_FLOAT,MPI_SUM,comm);
	}
	free(y);
	free(rc);
}
