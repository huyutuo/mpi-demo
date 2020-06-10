#include"myhead.h"

void sscan(MPI_Comm comm, int iam, float a, int root, float *b){
	MPI_Comm newcom;
	int color, key;

	if(iam <= root)
		color = 0;
	else
		color = MPI_UNDEFINED;
	key = iam;
	MPI_Comm_split(comm, color, key, &newcom);

	if(iam <= root){
		MPI_Reduce(&a, b,1, MPI_FLOAT, MPI_SUM,root,newcom);
		MPI_Comm_free(&newcom);
	}
	
	return;
}
