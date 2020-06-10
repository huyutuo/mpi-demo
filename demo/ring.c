#include "myhead.h"

void ring(int m, int *n, MPI_Comm comm, int np, int iam){
	int front,next;
	MPI_Status st,sts[2];
	MPI_Request sreq,rreq;
	MPI_Request reqs[2];
	front = (np+iam-1) % np;
	next = (np+iam+1) % np;
	
//	if(iam == 0) front = MPI_PROC_NULL;
//	if(iam == np-1) next = MPI_PROC_NULL;	
/*	if(iam % 2 == 0){
		MPI_Send(&m,1,MPI_INT,next,1,comm);	
		MPI_Recv(n,1,MPI_INT,front,1,comm,&st);
	}

	else {
		MPI_Recv(n,1,MPI_INT,front,1,comm,&st);	
		MPI_Send(&m,1,MPI_INT,next,1,comm);	
	}*/

//	MPI_Sendrecv(&m,1,MPI_INT,next,1,n,1,MPI_INT,front,1,comm,&st);
	MPI_Isend(&m,1,MPI_INT,next,1,comm,&reqs[0]);
	MPI_Irecv(n,1,MPI_INT,front,1,comm,&reqs[1]);
//	printf("WAIT%d",MPI_Waitall(2,reqs,sts));
//	MPI_Wait(&reqs[0],&sts[0]);	
//	MPI_Wait(&reqs[1],&sts[1]);	
	MPI_Request_free(reqs);
//	if(!MPI_Waitall(2,reqs,sts)){
//		MPI_Request_free(reqs);
//	}
	return ;
}
