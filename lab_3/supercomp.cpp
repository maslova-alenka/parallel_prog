#include <mpi.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <memory>
#include <algorithm>
#include <stdio.h>

const int ROOT_RANK = 0;


void generate_matrix(std::vector<int>& matrix, int size, int seed) {
    std::srand(seed);
    std::generate(matrix.begin(), matrix.end(), []() { return std::rand() % 750; });
}

void matrix_multiply(const std::vector<int>& A, const std::vector<int>& B,
    std::vector<int>& C, int size,
    int start_row, int end_row) {
    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < size; ++j) {
            C[i * size + j] = 0;
            for (int k = 0; k < size; ++k) {
                C[i * size + j] += A[i * size + k] * B[k * size + j];
            }
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    const std::vector<int> sizes = { 100, 250, 500, 750, 1000, 1500, 2000, 2500 };

    std::vector<int> A, B, C;

    std::vector<double> timings;

    if (rank == ROOT_RANK) {
        std::cout << "Starting matrix multiplication benchmark on "
            << num_procs << " processes\n";
    }

    for (int size : sizes) {
        if (rank == ROOT_RANK) {
            A.resize(size * size);
            B.resize(size * size);
            C.resize(size * size);

            generate_matrix(A, size, 42);
            generate_matrix(B, size, 123);
        }
        else {
            A.resize(size * size);
            B.resize(size * size);
        }

        MPI_Bcast(A.data(), size * size, MPI_INT, ROOT_RANK, MPI_COMM_WORLD);
        MPI_Bcast(B.data(), size * size, MPI_INT, ROOT_RANK, MPI_COMM_WORLD);

        int rows_per_proc = size / num_procs;
        int extra_rows = size % num_procs;
        int start_row = rank * rows_per_proc + std::min(rank, extra_rows);
        int end_row = start_row + rows_per_proc + (rank < extra_rows ? 1 : 0);

        std::vector<int> local_C(size * size, 0);

        MPI_Barrier(MPI_COMM_WORLD);
        double start = MPI_Wtime();

        matrix_multiply(A, B, local_C, size, start_row, end_row);

        if (rank == ROOT_RANK) {
            for (int i = start_row; i < end_row; ++i) {
                for (int j = 0; j < size; ++j) {
                    C[i * size + j] = local_C[i * size + j];
                }
            }

            for (int p = 1; p < num_procs; ++p) {
                int p_start = p * rows_per_proc + std::min(p, extra_rows);
                int p_end = p_start + rows_per_proc + (p < extra_rows ? 1 : 0);
                if (p_end > p_start) {
                    MPI_Recv(&C[p_start * size], (p_end - p_start) * size,
                        MPI_INT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
            }
        }
        else if (end_row > start_row) {
            MPI_Send(&local_C[start_row * size], (end_row - start_row) * size,
                MPI_INT, ROOT_RANK, 0, MPI_COMM_WORLD);
        }

        double end = MPI_Wtime();
        double duration = end - start;

        if (rank == ROOT_RANK) {
            timings.push_back(duration);
            std::cout << "Size: " << size << "x" << size
                << " | Time: " << duration << " sec"
                << " | Processes: " << num_procs << std::endl;
        }
    }

    if (rank == ROOT_RANK) {
        std::ofstream summary("performance_summary.csv");
        summary << "Matrix Size,Time(sec),Processes\n";
        for (size_t i = 0; i < sizes.size(); ++i) {
            summary << sizes[i] << "," << timings[i] << "," << num_procs << "\n";
        }
    }

    MPI_Finalize();
    return 0;
}