# Idea
Implementation of the parallel jacobi algorithm using MPI.

# How to run
Make sure MPI package is supported in your compiler. Run command will be
```
mpirun -np 9 jacobi 0.0001 1000
```
Where the first argument is tolerance for convergence, the second is the max iteration numbers.

# Jacobi algorithm

In numerical linear algebra, the Jacobi method is an iterative algorithm for determining the solutions of a strictly diagonally dominant system of linear equations.
Reference: [Wikipeida](https://en.wikipedia.org/wiki/Jacobi_method).