#include <gtest/gtest.h>
#include <cmath>
#include <vector>
#include "utilities.h"

TEST(SortByMagnitudeTest, SortByMagnitudeWorks) {
    std::vector<double> vec{3.0, -2.0, 1.0, 5.0, -4.0};
    sort_by_magnitude(vec);
    EXPECT_TRUE(std::abs(vec[0]) < std::abs(vec[1]));
    EXPECT_TRUE(std::abs(vec[1]) < std::abs(vec[2]));
    EXPECT_TRUE(std::abs(vec[2]) < std::abs(vec[3]));
    EXPECT_TRUE(std::abs(vec[3]) < std::abs(vec[4]));
}

TEST(ReadMatrixCsvTest, ReadMatrixCsvWorks) {
    TypedArray<TypedArray<double>> matrix = read_matrix_csv("test_matrix.csv");
    EXPECT_EQ(matrix.get(0).get(0), 1.0);
    EXPECT_EQ(matrix.get(0).get(1), 2.0);
    EXPECT_EQ(matrix.get(0).get(2), 3.0);
    EXPECT_EQ(matrix.get(1).get(0), 4.0);
    EXPECT_EQ(matrix.get(1).get(1), 5.0);
    EXPECT_EQ(matrix.get(1).get(2), 6.0);
}

TEST(WriteMatrixCsvTest, WriteMatrixCsvWorks) {
    TypedArray<TypedArray<double>> matrix;
    TypedArray<double> row1 = TypedArray<double>(3);
    row1.set(0, 1.0);
    row1.set(1, 2.0);
    row1.set(2, 3.0);
    TypedArray<double> row2 = TypedArray<double>(3);
    row2.set(0, 4.0);
    row2.set(1, 5.0);
    row2.set(2, 6.0);
    matrix.set(0, row1);
    matrix.set(1, row2);
    write_matrix_csv(matrix, "test_matrix_output.csv");

    TypedArray<TypedArray<double>> read_matrix = read_matrix_csv("test_matrix_output.csv");
    EXPECT_EQ(matrix.size(), read_matrix.size());
    EXPECT_EQ(matrix.get(0).size(), read_matrix.get(0).size());
    EXPECT_EQ(matrix.get(1).size(), read_matrix.get(1).size());
    EXPECT_EQ(matrix.get(0).get(0), read_matrix.get(0).get(0));
    EXPECT_EQ(matrix.get(0).get(1), read_matrix.get(0).get(1));
    EXPECT_EQ(matrix.get(0).get(2), read_matrix.get(0).get(2));
    EXPECT_EQ(matrix.get(1).get(0), read_matrix.get(1).get(0));
    EXPECT_EQ(matrix.get(1).get(1), read_matrix.get(1).get(1));
    EXPECT_EQ(matrix.get(1).get(2), read_matrix.get(1).get(2));
}

TEST(occurrence_map, CountsWordOccurrencesInTextFile) {
  map<string, int> word_count = occurrence_map("test_text_file.txt");
  EXPECT_EQ(word_count.size(), 5);
  EXPECT_EQ(word_count["wish"], 3);
  EXPECT_EQ(word_count["you"], 1);
  EXPECT_EQ(word_count["happy"], 2);
  EXPECT_EQ(word_count["every"], 2);
  EXPECT_EQ(word_count["days"], 2);
}


