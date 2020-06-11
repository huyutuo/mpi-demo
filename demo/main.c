#include "myhead.h"

#define maxnp 16
void main(int argc, char **argv){
	MPI_Comm  comm;
	int np,iam;
	int m,n,narray[100],marray[101];
	MPI_Datatype newtp, rect;
	MPI_Status st;
//	abc x[10];
	MPI_Aint sizeabc,extnewtp,extrect;
	int i, j,en;
	int offside;
	float a[31][57],b[53][59];
	float rhs[31],x[31];

	int rcounts[maxnp];
	int displs[maxnp];
	typedef struct{float a; int m;} floatint;
	floatint mxl,resmxl;

	FILE *fp;

	MPI_Comm rowcom, colcom;
	int rowid,colid;
#define chkdiagomal
	
	mybegin(&argc,&argv,&comm,&np,&iam);

#ifdef chkiteration
	// max_np = 6
	en = 5;
	n = en*np;
	for(i=0;i<n;i++){
		rhs[i] = i;
	}
	offside = iam*en;
	for(i=0;i<n;i++)
		for(j=0;j<en;j++){
			a[i][j] = 0.0;
			if(i ==(j+offside)) a[i][j] = 0.5;
		}
	iteration(comm, np,iam,n,en,&a[0][0],57,rhs,x,20);
	printf("%f,%f on proc %d\n",x[0],x[1],iam);
#endif

#ifdef chksscan
	a[0][0] = iam +1.0;
//	sscan(comm, iam, a[0][0],2,&b[0][0]);
//将通信子内各进程的同一个变量参与前缀规约计算，并将得到的结果发送回每个进程，
	MPI_Scan(&a[0][0], &b[0][0],1,MPI_FLOAT,MPI_SUM,comm);
//	if(iam == 2)
	printf("\nsum=%f in pros %d\n",b[0][0], iam);
#endif

#ifdef chkproc2d
	if(np<12) return;
	proc2d(comm,np,iam,3,4,&rowcom, &colcom,&rowid,&colid);
	printf("Proc %d = (%d,%d)\n",iam,rowid,colid);

#endif

#ifdef chkreduce
	mxl.a=(iam+1)*20;
	mxl.m = iam;
	MPI_Allreduce(&mxl,&resmxl,1,MPI_FLOAT_INT,MPI_MINLOC,comm);
	printf("\nA=%f,loc=%d in proc%d\n",resmxl.a,resmxl.m,iam);
#endif

#ifdef chkboast
	
	if(iam == 0){
		for(i=0;i<31;i++){
			for(j=0;j<57;j++){
				a[i][j] = i+j;
			}
		}
	}
	MPI_Bcast(&a[0][0],5,MPI_FLOAT,0,comm);
//	printf("\nA=%f,%f,%fin pros%d\n",a[0][0],a[0][1],a[0][2],iam);
	MPI_Scatter(&a[0][0],5,MPI_FLOAT,&a[1][0],5,MPI_FLOAT,1,comm);
//	printf("\nA=%f,%f,%fin pros%d\n",a[1][0],a[1][1],a[1][2],iam);
	MPI_Allgather(&a[1][0],5,MPI_FLOAT,&a[2][0],5,MPI_FLOAT,comm);
//	printf("\nA=%f,%f,%fin pros%d\n",a[2][0],a[2][1],a[2][2],iam);
	MPI_Alltoall(&a[0][0],5,MPI_FLOAT,&a[1][0],5,MPI_FLOAT,comm);
	MPI_Reduce(&a[1][0],&a[0][0],1,MPI_FLOAT,MPI_SUM,0,comm);
//	printf("\nA=%f,%f,%fin pros%d\n",a[1][0],a[1][1],a[1][2],iam);
	printf("\nA=%f,%f,%fin pros%d\n",a[0][0],a[1][1],a[1][2],iam);
/*	for(i=0;i<31;i++){
		for(j=0;j<57;j++){
			a[i][j] = i+j;
		}
	}
	j=iam*5;
	MPI_Gather(&a[0][j],3,MPI_FLOAT,&a[1][0],3,MPI_FLOAT,0,comm);
	printf("\nA=%f,%f,%f,%f,%f,%fin pros%d\n",\
		a[1][0],a[1][1],a[1][2],
		a[1][3],a[1][4],a[1][5],iam);*/
#endif

#ifdef chkdiagomal
	diagomal(2,3,57,&rect,&newtp);
	MPI_Type_commit(&rect);
	MPI_Type_commit(&newtp);
	MPI_Type_extent(rect,&extrect);
	MPI_Type_extent(newtp,&extnewtp);
	printf("\nsizeof=%ld and extent=%ld\n",extrect,extnewtp);

	if(iam == 0){
		for(i=0;i<31;i++){
			for(j=0;j<57;j++){
				a[i][j] = i+j;
			}
		}
		MPI_Send(a,3,newtp,1,5,comm);
	}
	if(iam == 1){
		MPI_Recv(a,3,newtp,0,5,comm,&st);
		printf("\nA=%f,%f,%f,%f,%f,%f,%f,%f",\
			a[0][0],a[0][1],a[1][0],a[1][1],\
			a[2][0],a[2][1],a[2][3],a[2][4]);
	}
	MPI_Type_free(&rect);
	MPI_Type_free(&newtp);
#endif

#ifdef chkstruct
	mpistruct(&newtp);
	MPI_Type_commit(&newtp);
	if(iam == 0){
		for(m=0;m<10;m++){
			x[m].a = m;
			x[m].b[0] = 20.0*(m+1);
			x[m].b[1] = 30.0*(m+1);
			x[m].c[0] = 'a'+3*m;
			x[m].c[1] = 'b'+3*m;
			x[m].c[2] = 'c'+3*m;
		}
		MPI_Send(x,3,newtp,1,5,comm);
		sizeabc = sizeof(abc);
		MPI_Type_extent(newtp,&extnewtp);
		printf("\nsizeof=%ld,and extend = %ld\n",sizeabc,extnewtp);

	}
	if(iam == 1){
		MPI_Recv(x,3,newtp,0,5,comm,&st);
		printf("\nvalues are %d, %f, %f, %c, %c, %c \n",\
				x[0].a,x[0].b[0],x[0].b[1],x[0].c[0],x[0].c[1],x[0].c[2]);
	
		printf("\nvalues are %d, %f, %f, %c, %c, %c \n",\
				x[1].a,x[1].b[0],x[1].b[1],x[1].c[0],x[1].c[1],x[1].c[2]);
	}
#endif

#ifdef chkdatatype
	for(m=0;m<100;m++) narray[m]=m;
	datatype("ivce",&newtp);
	MPI_Type_commit(&newtp);
	if(iam == 0) MPI_Send(narray,1,newtp,1,5,comm);
	if(iam == 1){
	       	MPI_Recv(marray,1,newtp,0,5,comm,&st);
		printf("\nData on Pros%d are %d,%d,%d,%d,%d,%d\n",
				iam,marray[0],marray[1],marray[2],marray[3],marray[5],marray[6]);
	}
	MPI_Type_free(&newtp);
#endif
//	mybegin(&argc,&argv,&comm,&np,&iam);
//	MPI_init(&argc,&argv);
//	MPI_Comm_dup(MPI_COMM_WORLD, &comm);
//	MPI_Comm_size(comm,&np);
//	MPI_Comm_rank(comm,&iam);
	m=iam;
	n=100;
	ring(m, &n, comm, np, iam);
	printf("\nHello World from pros. %d and m=%d!\n",iam,n);
	
	myend();	
//	MPI_Finalize();
}
