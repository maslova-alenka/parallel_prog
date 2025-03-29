#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

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

std::vector<std::vector<int>> multiply_matrices(const std::vector<std::vector<int>>& matrix_1, const std::vector<std::vector<int>>& matrix_2) {
    int size = matrix_1.size();
    std::vector<std::vector<int>> matrix_3(size, std::vector<int>(size, 0));

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                matrix_3[i][j] += matrix_1[i][k] * matrix_2[k][j];
            }
        }
    }
    return matrix_3;
}

void write_result(const std::string& filepath, const std::vector<std::vector<int>>& matrix_3, int size, clock_t start, clock_t end) {
    std::ofstream data(filepath);
    if (!data.is_open()) {
        std::cerr << "Unable to open file: " << filepath << std::endl;
        return;
    }

    for (const auto& row : matrix_3) {
        for (const int& element : row) {
            data << element << " ";
        }
        data << std::endl;
    }
    std::cout << "Matrix has been created: " << filepath << std::endl;
    double duration = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    std::cout << "Time taken: " << duration << " seconds" << std::endl;
}

int main() {
    std::string base_dir = "C:/Repository/lab_parallel_prog/lab_1/matrix";
    fs::create_directory(base_dir);

    std::vector<int> sizes = { 100, 250, 500, 750, 1000, 1500, 2000, 2500 };

    std::ofstream result("C:/Repository/lab_parallel_prog/lab_1/result.txt");

    for (int size : sizes) {
        std::string size_dir = base_dir + "/matrix_" + std::to_string(size);
        fs::create_directory(size_dir);

        std::string file1 = size_dir + "/m_1_" + std::to_string(size) + ".txt";
        std::string file2 = size_dir + "/m_2_" + std::to_string(size) + ".txt";
        std::string result_file = size_dir + "/res_" + std::to_string(size) + ".txt";

        std::vector<std::vector<int>> matrix_1 = create_rand_matrix(size);
        write_matrix_to_file(file1, matrix_1);
        std::vector<std::vector<int>> matrix_2 = create_rand_matrix(size);
        write_matrix_to_file(file2, matrix_2);

        matrix_1 = read_matrix_from_file(file1, size);
        matrix_2 = read_matrix_from_file(file2, size);

        clock_t start = clock();
        std::vector<std::vector<int>> matrix_3 = multiply_matrices(matrix_1, matrix_2);
        clock_t end = clock();

        write_result(result_file, matrix_3, size, start, end);
        result << "Size: " << size << " Time: " << (double(end - start)) / (double(CLOCKS_PER_SEC)) << std::endl;
    }
    result.close();

	return 0;
}