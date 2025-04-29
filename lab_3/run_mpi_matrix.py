import subprocess
import os


def run_mpi_exec(process_counts, executable):
    for n in process_counts:
        command = f"mpiexec -n {n} {executable}"
        print(f"Запуск с {n} процессами: {command}")
        try:
            subprocess.run(command, shell=True, check=True)
            print(f"Выполнение с {n} процессами завершено успешно")
        except subprocess.CalledProcessError as e:
            print(f"Ошибка при выполнении с {n} процессами: {e}")


if __name__ == "__main__":
    process_counts = [1, 2, 3, 4]
    run_mpi_exec(process_counts, "C:/Repository/lab_parallel_prog/lab3/out/build/x64-Debug/MPI_Project.exe")
    print("Все запуски завершены")