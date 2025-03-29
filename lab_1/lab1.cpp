#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void write_matrix_to_file(std::string& filename, const std::vector<std::vector<int>>& matrix) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    for (const auto& row : matrix) {
        for (const int& element : row) {
            file << element << " ";
        }
        file << std::endl;
    }
    std::cout << "Matrix has been written to file: " << filename << std::endl;
}

std::vector<std::vector<int>> create_rand_matrix(int size) {
    std::vector<std::vector<int>> matrix(size, std::vector<int>(size));

    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix.size(); j++) {
            matrix[i][j] = rand() % 750;
        }
    }
    return matrix;
}

std::vector<std::vector<int>> read_matrix_from_file(std::string& filepath, int size) {
    std::vector<std::vector<int>> matrix(size, std::vector<int>(size));
    std::ifstream data(filepath);
    if (!data.is_open()) {
        std::cerr << "Unable to open file: " << filepath << std::endl;
        return matrix;
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            data >> matrix[i][j];
        }
    }
    return matrix;
}

int main() {
    std::vector<int> sizes = { 100, 250, 500, 750, 1000, 1500, 2000, 2500 };

	return 0;
}