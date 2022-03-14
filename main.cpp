#include <iostream>
#include <fstream>
#include "math.h"
#include "mpi.h"
#include "jacobiGrid.h"
#include "dataIO.h"
using namespace std;

int main(int argc, char* argv[])
{
	size_t n = 0; // dimension of input
	size_t n_bar; // dimension of local matrix
	double tol; // tolerance
	int max_iter; // max iteration #
	bool conv; // whether converge
	int p; // total # of processes
	int q; // row # of the grid
	int world_rank; // rank of process in world

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	
	// setup grid
	jacobiGrid grid;
	q = int(sqrt(p));
	grid.setup(MPI_COMM_WORLD, q);

	// argument check
	if (argc != 3)
	{
		if (grid.isRoot()) { cout << "Argument number wrong!" << endl; }
		return 1;
	}

	// argument transform
	tol = atof(argv[1]);
	max_iter = atoi(argv[2]);

	// set up vector
	vector<double> A;
	vector<double> b;

	// process 0 reads data
	if (grid.isRoot())
	{
		// read the file
		ifstream is_A("./data/A_6.txt");
		ifstream is_b("./data/b_6.txt");

		// load
		load_matrix(&is_A, &A);
		load_vector(&is_b, &b);
		n = b.size();
		print_matrix(A, n);
		print_vector(b);

		// close the file
		is_A.close();
		is_b.close();
	}
	else
	{
		A.push_back(0);
		b.push_back(0);
	}
	// broadcast size and local size
	MPI_Bcast(&n, 1, MPI_INT, 0, grid.cart_comm);
	n_bar = n / q;
	grid.setN(n);
	grid.setN_bar(n_bar);

	// distribute data
	grid.distributeVec(&b);
	grid.distributeMat(&A);
	conv = grid.jacobi(tol, max_iter);

	// shut down mpi and exit
	MPI_Finalize();
	return 0;
}