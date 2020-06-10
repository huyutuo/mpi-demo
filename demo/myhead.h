#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

typedef struct{int a; float b[2]; char c[3];}abc;

void mybegin(int*, char***, MPI_Comm*, int*, int*);
void myend();
void ring(int, int *, MPI_Comm, int, int);
void datatype(char *, MPI_Datatype * );
void mpistruct(MPI_Datatype *);
void diagomal(int,int,int,MPI_Datatype *, MPI_Datatype *);
void proc2d(MPI_Comm, int, int, int, int, MPI_Comm *, MPI_Comm *, int *, int *);
void sscan(MPI_Comm, int,float, int, float *);
void gemmv(int,int,float *,int,float *,float *);
void iteration(MPI_Comm, int, int, int,int, float *, int, float *, float *, int);
