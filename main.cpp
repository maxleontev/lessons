#include <stdio.h>
#include <mpi.h>
//-------------------------------------------------------------------
int main(int argc, char ** argv) {

    int rank, size;
    MPI::Init(argc, argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("process %d of %d\r\n", rank, size);

    MPI::Finalize();

    return 0;
}
//-------------------------------------------------------------------
