#include "utilities.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <vector>
#include <cmath>
#include <unordered_map>

void sort_by_magnitude(std::vector<double>& vec) {
    std::sort(vec.begin(), vec.end(), [](double x, double y) {
        return std::abs(x) < std::abs(y);
    });
}

using namespace std;

TypedArray<TypedArray<double>> read_matrix_csv(const string path) {
  ifstream file(path);
  if (!file.is_open()) {
    throw runtime_error("Cannot open file: " + path);
  }

  TypedArray<TypedArray<double>> matrix;

  string line;
  while (getline(file, line)) {
    TypedArray<double> row_data;
    stringstream ss(line);

    string cell;
    while (getline(ss, cell, ',')) {
      double value;
      try {
        value = stod(cell);
      } catch (const exception& e) {
        throw runtime_error("Invalid value in CSV: " + cell);
      }
      row_data.set(row_data.size(), value);
    }

    if (matrix.size() > 0 && row_data.size() != matrix.get(0).size()) {
      throw runtime_error("Inconsistent number of columns in CSV");
    }

    matrix.set(matrix.size(), row_data);
  }

  return matrix;
}




void write_matrix_csv(const TypedArray<TypedArray<double>> &matrix, const std::string path) {
    std::ofstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    for (int i = 0; i < matrix.size(); i++) {
        const TypedArray<double>& row = matrix.safe_get(i);
        for (int j = 0; j < row.size(); j++) {
            file << row.safe_get(j);
            if (j < row.size() - 1) {
                file << ",";
            }
        }
        file << std::endl;
    }

    file.close();
}

map<string, int> occurrence_map(const string path) {
    map<string, int> freq_map;
    ifstream file(path);

    if (!file.is_open()) {
        throw runtime_error("Failed to open file " + path);
    }

    string word;
    while (file >> word) {
        string lowercase_word;

        for (char c : word) {
            if (c == '"' || c == '\'') {
                continue; // ignore quotes
            }

            if (!isalnum(c)) {
                lowercase_word += ' '; // replace non-alphanumeric characters with space
            } else {
                lowercase_word += tolower(c); // convert to lowercase
            }
        }

        istringstream iss(lowercase_word);
        string token;
        while (iss >> token) {
            if (!token.empty()) {
                freq_map[token]++;
            }
        }
    }

    file.close();
    return freq_map;
}


