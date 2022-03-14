#ifndef LOADDATA_H
#define LOADDATA_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void load_matrix(std::istream*, std::vector<double>*);

void load_vector(std::istream*, std::vector<double>*);

void print_matrix(std::vector<double>, int);

void print_vector(std::vector<double>);

#endif