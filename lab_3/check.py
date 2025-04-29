import matplotlib.pyplot as plt
import numpy as np

PATH = 'C:/Repository/lab_parallel_prog/lab3/matrix/'
PATH_2 = 'C:/Repository/lab_parallel_prog/lab3/'
PATH_3 = 'C:/Repository/lab_parallel_prog/lab3/result/'
PATH_4 = 'C:/Repository/lab_parallel_prog/lab3/supercomp/'


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
            

def plot_time_vs_matrix_size(filenames, labels=None):
    plt.figure(figsize=(10, 6))
    
    if labels is None:
        labels = [f'File {i+1}' for i in range(len(filenames))]
    
    for filename, label in zip(filenames, labels):
        sizes, times = [], []
        with open(filename, 'r') as file:
            for line in file:
                parts = line.split()
                size = int(parts[1])
                time = float(parts[4])
                sizes.append(size)
                times.append(time)
        
        plt.plot(sizes, times, marker='*', markersize=10, linestyle='-', label=label)
    
    plt.title('Зависимость времени от размера матрицы')
    plt.xlabel('Размер матрицы')
    plt.ylabel('Время (с)')
    plt.xticks(sizes) 
    plt.legend()
    plt.show()


if __name__ == "__main__":
    matrix_sizes = [100, 250, 500, 750, 1000, 1500, 2000, 2500]
    results = []

    for size in matrix_sizes:
        is_correct = check_matrix_multiplication(size)
        results.append((size, is_correct))
        write_results_to_file(size, results, PATH_2 + "check.txt")

    
    files = [PATH_3 + "proc1.txt", PATH_3 + "proc2.txt", PATH_3 + "proc3.txt", PATH_3 + "proc4.txt"]
    labels = ["Processes 1", "Processes 2", "Processes 3", "Processes 4"] 
    plot_time_vs_matrix_size(files, labels)

    files = [PATH_4 + "proc1.txt", PATH_4 + "proc2.txt", PATH_4 + "proc4.txt", PATH_4 + "proc6.txt", PATH_4 + "proc8.txt", 
             PATH_4 + "proc10.txt", PATH_4 + "proc12.txt", PATH_4 + "proc16.txt", PATH_4 + "proc20.txt"]
    labels = ["Processes 1", "Processes 2", "Processes 4", "Processes 6", "Processes 8", "Processes 10", "Processes 12", "Processes 16", "Processes 20"] 
    plot_time_vs_matrix_size(files, labels)