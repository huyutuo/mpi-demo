#include "myhead.h"

void mybegin(int *argc,char ***argv,MPI_Comm *comm,int *np,int *iam){
	MPI_Init(argc,argv);
	MPI_Comm_dup(MPI_COMM_WORLD,comm);
	MPI_Comm_size(*comm,np);
	MPI_Comm_rank(*comm,iam);
	
	return ;
}
