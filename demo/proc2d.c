#include "myhead.h"

void proc2d(MPI_Comm comm, int np,int iam,int p,int q,MPI_Comm *rowcom,MPI_Comm *colcom,int *rowid,int *colid){
	int color, key, pxq;
	MPI_Comm valcom;

	pxq = p*q;
	if(np<pxq){return ;}
	
	if(iam<pxq) color=0;
	else color = MPI_UNDEFINED;
	key = iam;
	MPI_Comm_split(comm, color,key,&valcom);
	
	if(valcom == MPI_COMM_NULL) return;
	color = iam/q;
	MPI_Comm_split(valcom, color,key,rowcom);

	color = iam%q;
	MPI_Comm_split(valcom, color,key,colcom);

	MPI_Comm_rank( *colcom, rowid);
	MPI_Comm_rank( *rowcom, colid);

}
