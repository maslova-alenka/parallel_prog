import matplotlib.pyplot as plt
import numpy as np

PATH = 'C:/Repository/lab_parallel_prog/lab_1/matrix/'
PATH_2 = 'C:/Repository/lab_parallel_prog/lab_1/'




def read_matrix_from_file(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()
        matrix = np.array([list(map(int, line.split())) for line in lines])
    return matrix


def check_matrix_multiplication(size):
    size_m = str(size)
    matrix_1 = read_matrix_from_file(PATH + f"matrix_{size_m}/m_1_{size_m}.txt")
    matrix_2 = read_matrix_from_file(PATH + f"matrix_{size_m}/m_2_{size_m}.txt")
    result_numpy = np.dot(matrix_1, matrix_2)
    result_file = read_matrix_from_file(PATH + f"matrix_{size_m}/res_{size_m}.txt")
    return np.array_equal(result_numpy, result_file)


def write_results_to_file(size, results, filename):
    with open(filename, 'w') as file:
        for size, is_correct in results:
            if is_correct:
                status = 'Правильно'
            else:
                status = 'Ошибка'
            file.write(f"Размер {size}x{size}: {status}\n")
            

def plot_time_vs_matrix_size(filename):
    sizes, times = [], []
    
    # with open(filename, 'r') as file:
    #     for line in file:
    #         parts = line.split(", ")
    #         size_part = parts[0].split(": ")[1]
    #         time_part = parts[1].split(": ")[1].replace(" seconds", "")
    #         sizes.append(int(size_part.split("x")[0]))
    #         times.append(float(time_part))

    # sizes = []
    # times = []
    with open(filename, 'r') as file:
        for line in file:
            parts = line.split()
            size = int(parts[1])
            time = float(parts[3])
            sizes.append(size)
            times.append(time)

    plt.figure(figsize=(10, 6))
    plt.plot(sizes, times, marker='*', markersize=10, linestyle='-', color='lime')
    plt.title('Зависимость времени от размера матрицы')
    plt.xlabel('Размер матрицы')
    plt.ylabel('Время (с)')
    plt.xticks(sizes)
    plt.show()


if __name__ == "__main__":
    matrix_sizes = [100, 250, 500, 750, 1000, 1500, 2000, 2500]
    results = []

    for size in matrix_sizes:
        is_correct = check_matrix_multiplication(size)
        results.append((size, is_correct))
        write_results_to_file(size, results, PATH_2 + "check.txt")

    plot_time_vs_matrix_size(PATH_2 + "result.txt")
