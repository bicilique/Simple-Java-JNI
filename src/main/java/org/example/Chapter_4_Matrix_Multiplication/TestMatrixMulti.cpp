#include <iostream>
#include <vector>

void multiplyMatrix(const std::vector<std::vector<int> >& matrix1, const std::vector<std::vector<int> >& matrix2, std::vector<std::vector<int> >& result) {
    size_t rows1 = matrix1.size();
    size_t cols1 = matrix1[0].size();
    size_t rows2 = matrix2.size();
    size_t cols2 = matrix2[0].size();

    // Validation: number of columns in matrix1 should equal number of rows in matrix2
    if (cols1 != rows2) {
        std::cerr << "Matrix dimensions are not suitable for multiplication!" << std::endl;
        return;
    }

    // Initialize result matrix with zeros
    result.resize(rows1, std::vector<int>(cols2, 0));

    // Matrix multiplication
    for (size_t i = 0; i < rows1; ++i) {
        for (size_t j = 0; j < cols2; ++j) {
            for (size_t k = 0; k < cols1; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

int main() {
    // Example matrices
    std::vector<std::vector<int> > matrix1(2, std::vector<int>(2));
    matrix1[0][0] = 1; matrix1[0][1] = 2;
    matrix1[1][0] = 3; matrix1[1][1] = 4;

    std::vector<std::vector<int> > matrix2(2, std::vector<int>(2));
    matrix2[0][0] = 5; matrix2[0][1] = 6;
    matrix2[1][0] = 7; matrix2[1][1] = 8;

    std::vector<std::vector<int> > result;

    // Call the multiplication function
    multiplyMatrix(matrix1, matrix2, result);

    // Output the result matrix
    std::cout << "Resulting matrix:" << std::endl;
    for (size_t i = 0; i < result.size(); ++i) {
        for (size_t j = 0; j < result[i].size(); ++j) {
            std::cout << result[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
