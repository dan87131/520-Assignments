#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include "typed_array.h"

using namespace std;

// Sorts a vector of doubles by absolute magnitude
void sort_by_magnitude(vector<double> &vec);

// Reads a comma separated value (CSV) file of doubles into a matrix and returns it
TypedArray<TypedArray<double>> read_matrix_csv(const string path);

// Writes a comma separated value (CSV) file of doubles from a matrix
void write_matrix_csv(const TypedArray<TypedArray<double>> &matrix, const string path);

// Reads in an ascii text file and returns an association where each key is a word in the text file,
// and each value is the number of occurrences of that word
map<string, int> occurrence_map(const string path);

#endif // UTILITIES_H