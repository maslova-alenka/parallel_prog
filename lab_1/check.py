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
    
    with open(filename, 'r') as file:
        for line in file:
            parts = line.split(", ")
            size_part = parts[0].split(": ")[1]
            time_part = parts[1].split(": ")[1].replace(" seconds", "")
            sizes.append(int(size_part.split("x")[0]))
            times.append(float(time_part))

    plt.figure(figsize=(10, 6))
    plt.plot(sizes, times, marker='o', linestyle='-', color='b')
    plt.title('Зависимость времени от размера матрицы')
    plt.xlabel('Размер матрицы (строки x столбцы)')
    plt.ylabel('Время (секунды)')
    plt.xticks(sizes)
    plt.grid()
    plt.savefig('time_vs_size.png')
    plt.show()


if __name__ == "__main__":
    # matrix_sizes = [100]
    matrix_sizes = [100, 250, 500, 750, 1000, 1500, 2000, 2500]
    results = []

    matrix = read_matrix_from_file(PATH + f"matrix_100/m_1_100.txt")
    if matrix is not None:

        print(matrix)
    else:
        print("Не удалось прочитать матрицу из файла.")

    
    matrix2 = read_matrix_from_file(PATH + f"matrix_100/m_2_100.txt")
    if matrix2 is not None:

        print(matrix2)
    else:
        print("Не удалось прочитать матрицу из файла.")



    for size in matrix_sizes:
        print("начался цикл {size}")
        is_correct = check_matrix_multiplication(size)
        print(is_correct)
        results.append((size, is_correct))
        print(results)
        write_results_to_file(size, results, PATH_2 + "check.txt")
        print("записано")

    plot_time_vs_matrix_size("results.txt")
