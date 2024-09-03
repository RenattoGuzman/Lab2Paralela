#include <iostream>
#include <fstream>
#include <cstdlib>  // Para rand() y srand()
#include <ctime>    // Para time()
#include <algorithm> // Para std::sort

#define FILENAME "numeros.csv"
#define SORTED_FILENAME "numeros_ordenados.csv"

int compare (const void * a, const void * b) {
    return (*(int*)a - *(int*)b);
}

int main() {
    int n;

    // Pedir al usuario el número de elementos
    std::cout << "Ingrese la cantidad de números aleatorios a generar: ";
    std::cin >> n;

    // Reservar memoria en el heap para el arreglo de números
    int* Array = new int[n];

    // Generar números aleatorios y escribirlos en un archivo
    std::ofstream escribirNumeros(FILENAME);
    if (!escribirNumeros) {
        std::cerr << "Error al crear el archivo " << FILENAME << std::endl;
        return 1;
    }

    srand(time(0));  // Semilla para la generación de números aleatorios
    for (int i = 0; i < n; ++i) {
        Array[i] = rand() % 1000;  // Genera un número aleatorio entre 0 y 999
        escribirNumeros << Array[i];
        if (i < n - 1) escribirNumeros << ",";
    }
    escribirNumeros.close();

    // Leer los números desde el archivo
    std::ifstream leerNumeros(FILENAME);
    if (!leerNumeros) {
        std::cerr << "Error al leer el archivo " << FILENAME << std::endl;
        delete[] Array;  // Liberar la memoria reservada
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

    // Escribir los números ordenados en un segundo archivo
    std::ofstream escribirNumerosOrdenados(SORTED_FILENAME);
    if (!escribirNumerosOrdenados) {
        std::cerr << "Error al crear el archivo " << SORTED_FILENAME << std::endl;
        delete[] Array;  // Liberar la memoria reservada
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        escribirNumerosOrdenados << Array[i];
        if (i < n - 1) escribirNumerosOrdenados << ",";
    }
    escribirNumerosOrdenados.close();

    // Liberar la memoria reservada
    delete[] Array;

    std::cout << "Proceso completado. Los números generados se encuentran en " << FILENAME << " y los números ordenados en " << SORTED_FILENAME << std::endl;

    return 0;
}
