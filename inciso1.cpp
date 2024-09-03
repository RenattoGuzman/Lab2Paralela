#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <chrono>

#define FILENAME "numeros.csv"
#define SORTED_FILENAME "numeros_ordenados.csv"

int compare (const void * a, const void * b) {
    return (*(int*)a - *(int*)b);
}

int main() {
    int n;

    std::cout << "Ingrese la cantidad de números aleatorios a generar: ";
    std::cin >> n;

    // reservar memoria en el heap
    int* Array = new int[n];

    // Comienza a medir el tiempo
    auto start_time = std::chrono::high_resolution_clock::now();

    // generar números
    std::ofstream escribirNumeros(FILENAME);
    if (!escribirNumeros) {
        std::cerr << "Error al crear el archivo " << FILENAME << std::endl;
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        Array[i] = rand() % 1000;
        escribirNumeros << Array[i];
        if (i < n - 1) escribirNumeros << ",";
    }
    escribirNumeros.close();

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> crear_numeros_time = end_time - start_time;
    std::cout << "Tiempo de generación y escritura: " << crear_numeros_time.count() << " segundos." << std::endl;

    // comienza a medir el tiempo
    auto start_time2 = std::chrono::high_resolution_clock::now();

    // Leer desde el archivo
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

    // Ordenar los números
    std::qsort(Array, n, sizeof(int), compare);

    auto end_time2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> ordenar_numeros_time = end_time2 - start_time2;
    std::cout << "Tiempo de ordenación: " << ordenar_numeros_time.count() << " segundos." << std::endl;

    // guardar números ordenados en otro archivo
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

    // liberar la memoria reservada
    delete[] Array;

    std::cout << "Proceso completado. Los números generados se encuentran en " << FILENAME << " y los números ordenados en " << SORTED_FILENAME << std::endl;

    return 0;
}
