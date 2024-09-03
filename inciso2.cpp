#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <omp.h>

#define FILENAME "numeros.csv"
#define SORTED_FILENAME "numeros_ordenados.csv"

int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void parallelQuickSort(int* arr, int left, int right) {
    if (left < right) {
        int pivot = arr[right];
        int i = left - 1;
        for (int j = left; j < right; j++) {
            if (arr[j] < pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[right]);
        int partitionIndex = i + 1;

        #pragma omp parallel sections
        {
            #pragma omp section
            parallelQuickSort(arr, left, partitionIndex - 1);
            #pragma omp section
            parallelQuickSort(arr, partitionIndex + 1, right);
        }
    }
}

int main() {
    int n;
    double start_time, end_time;

    std::cout << "Ingrese la cantidad de números aleatorios a generar: ";
    std::cin >> n;

    int* Array = new int[n];

    // Medir tiempo de generación y escritura de números aleatorios
    start_time = omp_get_wtime();

    std::ofstream escribirNumeros(FILENAME);
    if (!escribirNumeros) {
        std::cerr << "Error al crear el archivo " << FILENAME << std::endl;
        delete[] Array;
        return 1;
    }

    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        Array[i] = rand() % 1000;
        #pragma omp critical
        {
            escribirNumeros << Array[i];
            if (i < n - 1) escribirNumeros << ",";
        }
    }
    escribirNumeros.close();

    end_time = omp_get_wtime();
    std::cout << "Tiempo de generación y escritura: " << end_time - start_time << " segundos.\n";

    std::ifstream leerNumeros(FILENAME);
    if (!leerNumeros) {
        std::cerr << "Error al leer el archivo " << FILENAME << std::endl;
        delete[] Array;
        return 1;
    }

    int i = 0;
    std::string numero;
    while (getline(leerNumeros, numero, ',')) {
        Array[i++] = std::stoi(numero);
    }
    leerNumeros.close();

    // Medir tiempo de ordenación
    start_time = omp_get_wtime();

    parallelQuickSort(Array, 0, n - 1);

    end_time = omp_get_wtime();
    std::cout << "Tiempo de ordenación: " << end_time - start_time << " segundos.\n";

    std::ofstream escribirNumerosOrdenados(SORTED_FILENAME);
    if (!escribirNumerosOrdenados) {
        std::cerr << "Error al crear el archivo " << SORTED_FILENAME << std::endl;
        delete[] Array;
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        escribirNumerosOrdenados << Array[i];
        if (i < n - 1) escribirNumerosOrdenados << ",";
    }
    escribirNumerosOrdenados.close();

    delete[] Array;

    std::cout << "Proceso completado. Los números generados se encuentran en " << FILENAME << " y los números ordenados en " << SORTED_FILENAME << std::endl;

    return 0;
}
