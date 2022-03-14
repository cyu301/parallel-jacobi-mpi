#ifndef JACOBIGRID_H
#define JACOBIGRID_H

#include "cart2dGrid.h"
#include "math.h"
#include <vector>
#include <iostream>
#include <algorithm>

class jacobiGrid : public cart2dGrid {
public:
	jacobiGrid();
	~jacobiGrid();
	int getTRank();
	void setN(size_t);
	void setN_bar(size_t);
	void distributeVec(std::vector<double>*);
	void distributeMat(std::vector<double>*);
	bool jacobi(double, int);

protected:
	bool isDiag();
	void displayLocalVec(char);
	void displayLocalMat(char);
	double* local_A;
	double* local_b;
	size_t n;
	size_t n_bar;
};

#endif // !JACOBIGRID_H
