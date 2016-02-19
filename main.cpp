#include <stdio.h>
#include <mpi.h>
//-------------------------------------------------------------------
int main(int argc, char ** argv) {
    char mess[20];

    int rank, size , type = 99, status;
    MPI::Init(argc, argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0) {
        sprintf(mess, "hello, world!");
        for(int i=1; i<size; i++)
            MPI_Send(mess, 14, MPI_CHAR, i, type, MPI_COMM_WORLD);
    } else {
        MPI_Status st;
        MPI_Recv(mess, 20, MPI_CHAR, 0, type, MPI_COMM_WORLD, &st);
        printf("%s\r\n", mess);
    }

    MPI::Finalize();

    return 0;
}
//-------------------------------------------------------------------
