#ifndef CART2DGRID_H
#define CART2DGRID_H

#include "mpi.h"

class  cart2dGrid
{
public:
    MPI_Comm  row_comm;  // Communicator for my row
    MPI_Comm  col_comm;  // Communicator for my col   
    MPI_Comm  cart_comm; // Communicator for entire grid 
    int       my_row;    // My row number         
    int       my_col;    // My column number      
    int       my_rank;   // My rank in the grid comm    

    cart2dGrid();
    void setup(MPI_Comm, int);
    bool isRoot();

protected:
    int       p;         // Total number of processes  
    int       row_len;   // Row length of grid        
    int       col_len;   // Column length of grid       
};

#endif