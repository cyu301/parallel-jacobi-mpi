#include "jacobiGrid.h"
using namespace std;

jacobiGrid::jacobiGrid()
{
	cart2dGrid();
	local_A = NULL;
	local_b = NULL;
	n = -1;
	n_bar = -1;
}

jacobiGrid::~jacobiGrid()
{
	delete[] local_A;
	delete[] local_b;
}

bool jacobiGrid::isDiag()
{
	return my_col == my_row;
}

int jacobiGrid::getTRank()
{
	return my_col * row_len + my_row;
}

void jacobiGrid::setN(size_t n_)
{
	n = n_;
}

void jacobiGrid::setN_bar(size_t n_bar_)
{
	n_bar = n_bar_;
}

void jacobiGrid::distributeVec(vector <double>* b_)
{
	local_b = new double[n_bar];
	if (my_col == 0)
	{
		MPI_Scatter(&(*b_)[0], n_bar, MPI_DOUBLE, local_b, n_bar, MPI_DOUBLE, 0, col_comm);
	}
	MPI_Bcast(local_b, n_bar, MPI_DOUBLE, 0, row_comm);
}

void jacobiGrid::distributeMat(vector <double>* A_)
{
	local_A = new double[n_bar * n_bar + n_bar];
	double* temp = new double[n_bar * n];
	if (my_col == 0)
	{
		MPI_Scatter(&(*A_)[0], n_bar*n, MPI_DOUBLE, temp, n_bar*n, MPI_DOUBLE, 0, col_comm);
	}
	for (size_t i = 0; i < n_bar; ++i)
	{
		MPI_Scatter(&temp[n * i], n_bar, MPI_DOUBLE, &local_A[n_bar * i], n_bar, MPI_DOUBLE, 0, row_comm);	
	}
	if(isDiag())
	{
		for (size_t i = 0; i < n_bar; ++i)
		{
			local_A[n_bar * n_bar + i] = local_A[i * n_bar + i];
		}
	}
	MPI_Bcast(&local_A[n_bar * n_bar], n_bar, MPI_DOUBLE, my_row, row_comm);
	delete[] temp;
}

bool jacobiGrid::jacobi(double tol, int max_iter)
{
	bool converged = false; // converged flag
	double diff = 0; // local difference in each iteration
	double diff_glo = 0; // global difference
	int count_iter = 0;
	MPI_Status status;

	double* x_new = new double[n_bar]();
	double* x_old = new double[n_bar]();
	double* x_red = new double[n_bar](); // before reduce
	
	while (!converged && count_iter < max_iter)
	{
		// swap new and old
		swap(x_old, x_new);
		for (size_t i = 0; i < n_bar; ++i)
		{
			x_red[i] = local_b[i] / row_len;
		}

		// calculate local
		for (size_t i = 0; i < n_bar; ++i)
		{
			for (size_t j = 0; j < n_bar; ++j)
			{
				if (!isDiag() or i != j) 
				{
					x_red[i] -= local_A[i * n_bar + j] * x_old[j];
				}
			}
			x_red[i] /= local_A[n_bar * n_bar + i];
		}

		// all reduce in row
		MPI_Allreduce(x_red, x_new, n_bar, MPI_DOUBLE, MPI_SUM, row_comm);

		// talk to transpose
		if (!isDiag())
		{
			MPI_Sendrecv_replace(x_new, n_bar, MPI_DOUBLE,
				getTRank(), 0, getTRank(), 0, cart_comm, &status);
		}

		// diff for local x
		double sum = 0;
		for (size_t i = 0; i < n_bar; ++i)
		{
			sum += (x_old[i] - x_new[i]) * (x_old[i] - x_new[i]);
		}
		diff = sqrt(sum);
		MPI_Allreduce(&diff, &diff_glo, 1, MPI_DOUBLE, MPI_SUM, row_comm);

		converged = (diff_glo < tol);
		++count_iter;
	} // end of while

	// output info
	double* x_star_global =  new double[n];
	// use gather send to root process
	if (my_row == 0)
		MPI_Gather(x_new, n_bar, MPI_DOUBLE, x_star_global, n_bar, MPI_DOUBLE, 0, row_comm);
	if (isRoot() && converged)
	{
		cout << "Converged after " << count_iter << " iterations." << endl;
	}
	else if (isRoot() && !converged)
	{
		cout << "Does not converge after " << count_iter << " iterations." << endl;
	}
	if (isRoot())
	{
		cout << "Solution is:" << endl;
		for (size_t i = 0; i < n; ++i)
		{
			cout << x_star_global[i] << "\t";
		}
		cout << endl;
	}

	delete[] x_new;
	delete[] x_old;
	delete[] x_red;
	delete[] x_star_global;
	return converged;
}

void jacobiGrid::displayLocalVec(char a)
{
	cout << a << my_rank << "\t";
	for (size_t i = 0; i < n_bar; ++i)
	{
		cout << local_b[i] << "\t";
	}
	cout << endl;
}

void jacobiGrid::displayLocalMat(char a)
{
	cout << a << my_rank << "\t" << "\n";
	for (size_t i = 0; i < n_bar + 1; ++i)
	{
		for (size_t j = 0; j < n_bar; ++j)
		{
			cout << local_A[i*n_bar+j] << "\t";
		}
		cout << "\n";
	}
}