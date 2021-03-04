#include <iostream>
#include <mpi.h>

void check(int& rc) {
    if (rc != MPI_SUCCESS) {
	MPI_Abort(MPI_COMM_WORLD, 1);
    }
}

int main(int argc, char* argv[]) {
    int rc = MPI_Init(&argc, &argv);
    if (rc != MPI_SUCCESS) {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }    
    int rank;
    int size;
    rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    check(rc);
    rc = MPI_Comm_size(MPI_COMM_WORLD, &size);
    check(rc);
    if (rank == 0) {
	int m = 0;
	rc = MPI_Send(&m, 1, MPI_INT, rank+1, 1337, MPI_COMM_WORLD);
	check(rc);
	std::cout << "Process " << rank << " send message: " << m 
	<< "; to process " << rank+1 << "\n";
	MPI_Status st;
	rc = MPI_Recv(&m, 1, MPI_INT, size-1, 1337, MPI_COMM_WORLD, &st);
	check(rc);
	std::cout << "Process 0 got message: " << m << "; from porcess "
	 << size - 1 << "\n";
    }
    else {
	int  m;
	MPI_Status st;
	rc = MPI_Recv(&m, 1, MPI_INT, rank-1, 1337, MPI_COMM_WORLD, &st);
	check(rc);
	int count;
	rc = MPI_Get_count(&st, MPI_INT, &count);
	check(rc);
	std::cout <<"Process " << rank <<" got message: " << m << "; from process "
	<< rank-1 << "\n";
	m += rank;
	if (rank < size-1) {
	    rc = MPI_Send(&m, 1, MPI_INT, rank+1, 1337, MPI_COMM_WORLD);
	    check(rc);
	    std::cout << "Process " << rank << " send message: " << m << "; to process "
	    << rank+1 << "\n";
	}
	else {
	    rc = MPI_Send(&m, 1, MPI_INT, 0, 1337, MPI_COMM_WORLD);
	    std::cout << "Process " << rank << " send message: " << m << "; to process 0 "
	    << "\n";
	    check(rc);
	}
    }	
    MPI_Finalize();
    return 0;
}
