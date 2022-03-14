#include "cart2dGrid.h"

cart2dGrid::cart2dGrid()
{
    row_comm = MPI_COMM_NULL;
    col_comm = MPI_COMM_NULL;
    cart_comm = MPI_COMM_NULL;
    my_row = -1;
    my_col = -1;
    my_rank = -1;
    p = -1;
    row_len = -1;
    col_len = -1;
}

void cart2dGrid::setup(MPI_Comm parent_comm, int row_len_)
{
    int old_rank;
    int dimensions[2];
    int wrap_around[2];
    int coordinates[2];
    int free_coords[2];

    // Set up Global Grid Information 
    MPI_Comm_size(parent_comm, &p);
    MPI_Comm_rank(parent_comm, &old_rank);

    // Set row and column length
    row_len = row_len_;
    col_len = p / row_len;
    dimensions[0] = col_len;
    dimensions[1] = row_len;

    // No circular shift here
    wrap_around[0] = wrap_around[1] = 0;
    MPI_Cart_create(parent_comm, 2, dimensions, wrap_around, 1, &cart_comm);
    MPI_Comm_rank(cart_comm, &(my_rank));
    MPI_Cart_coords(cart_comm, my_rank, 2, coordinates);
    my_row = coordinates[0];
    my_col = coordinates[1];

    // Set up row communicators
    free_coords[0] = 0;
    free_coords[1] = 1;
    MPI_Cart_sub(cart_comm, free_coords, &row_comm);

    // Set up column communicators
    free_coords[0] = 1;
    free_coords[1] = 0;
    MPI_Cart_sub(cart_comm, free_coords, &col_comm);
}

bool cart2dGrid::isRoot()
{
    return my_rank == 0;
}