#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

struct ABC{
    int a;
    char b[2];
    float c[3];
};

void showABC(struct ABC abc)
{
    printf("ABC: a=%d\n", abc.a);
}

int boast[4][4];
void showBoast(int a[4][4])
{
    printf("--------------\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    printf("--------------\n");
}

void main(int argc, char **argv)
{

    int total_process_num;
    int process_index;
    // init
    MPI_Comm comm;
    MPI_Init(&argc, &argv);
	MPI_Comm_dup(MPI_COMM_WORLD, &comm);
	MPI_Comm_size(comm, &total_process_num);
	MPI_Comm_rank(comm, &process_index);
    printf("init success: total_process_num %d, process_index %d\n",
           total_process_num, process_index);

    // test construct new type
    MPI_Datatype new_type;
    struct ABC abc;
    // 对应abc中三种类型
    MPI_Datatype  old_type[3];
    old_type[0] = MPI_INT;
	old_type[1] = MPI_CHAR;
	old_type[2] = MPI_FLOAT;

    // abc中每种类型的个数
    int every_old_type_counts[3];
    every_old_type_counts[0] = 1;
    every_old_type_counts[1] = 2;
    every_old_type_counts[2] = 3;

    // 地址偏移
    MPI_Aint address_offset[3];
    MPI_Address(&abc.a, &address_offset[0]);
	MPI_Address(&abc.b[0], &address_offset[1]);
	MPI_Address(&abc.c[0], &address_offset[2]);

    address_offset[1] -= address_offset[0];
	address_offset[2] -= address_offset[0];
	address_offset[0] = 0;

    MPI_Type_struct(3, every_old_type_counts, address_offset, old_type, &new_type);

    // use new_type to send and recv
    MPI_Type_commit(&new_type);
    struct ABC test[3];
    MPI_Status recv_status;

    if (process_index == 0) {
        for (int i = 0; i < 3; i++){
            test[i].a = 1;
            test[i].b[0] = 'h';
            test[i].b[1] = 'y';
            test[i].c[0] = 1;
            test[i].c[1] = 2;
            test[i].c[2] = 3;
        }
        MPI_Send(test, 3, new_type, 1, 100, comm);
        showABC(test[0]);
        printf("process_index: %d, send to process 1\n", process_index);
    } else {
        int front = process_index - 1;
        int next = process_index + 1;
        if (process_index + 1 == total_process_num) {
            next = MPI_PROC_NULL;
        }
        MPI_Recv(test, 3, new_type, front, 100, comm, &recv_status);
        test[0].a += 1;
        showABC(test[0]);
        MPI_Send(test, 3, new_type, next, 100, comm);
        if (process_index + 1 != total_process_num) {
            printf("porcess_index: %d. recv from %d, send to %d\n",
               process_index, front, next);
        }
    }

    // test boast
    if (process_index == 0) {
        for (int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                boast[i][j] = i*4 + j; 
    }
    //MPI_Bcast(boast, 16, MPI_INT, 0, comm);
    MPI_Scatter(boast, 4, MPI_INT, &boast[process_index][0], 4, MPI_INT, 0, comm);
    showBoast(boast);

    int all_gather[4][4];
    MPI_Allgather(&boast[process_index][0], 4, MPI_INT, all_gather, 4, MPI_INT,comm);
    printf("process_index %d\n", process_index);
    showBoast(all_gather);

    //all to all
    int all_to_all[4];
    int recive[4];
    for(int i = 0; i < 4; i++)
        all_to_all[i] = process_index;
    MPI_Alltoall(all_to_all, 1, MPI_INT, recive, 1, MPI_INT, comm);
    printf("all_to_all: %d %d %d %d\n", recive[0], recive[1], recive[2], recive[3]);

    // reduce
    int sum = 0;
    MPI_Reduce(&process_index, &sum, 1, MPI_INT, MPI_SUM, 0,comm);
    printf("process_index %d, sum %d\n", process_index, sum);



    // finalize
    MPI_Finalize();
}