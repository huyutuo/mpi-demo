#include "myhead.h"

void datatype(char *cont, MPI_Datatype *newtp){
	int count = 3;
	int stride = 2, length = 1;
	int lengths[2]={3,2}, displs[2] = {0,5};
	if(cont[0] == 'c'){
		MPI_Type_contiguous(count,MPI_INT,newtp);
	}
	if(cont[0] == 'v'){
		MPI_Type_vector(count,length,stride,MPI_INT,newtp);
	}
	if(cont[0] == 'h'){
		stride = 8;
		MPI_Type_hvector(count,length,stride,MPI_INT,newtp);
	}

	if(cont[0] == 'i'){
		MPI_Type_indexed(count,lengths,displs,MPI_INT,newtp);
	}
//	MPI_Type_commit(newtp);
	return ;
}
