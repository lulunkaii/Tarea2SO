#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <cstring>
#include "algoritmos.h"

int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cerr << "Uso incorrecto: " << argv[0] << " -m <num-marcos-iniciales> -a <alg-reemplazo> -f <archivo-referencias>" << std::endl;
        return EXIT_FAILURE;
    }

    int num_marcos_iniciales = 0;
    std::string alg_reemplazo, archivo_referencias;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-m") == 0) {
            num_marcos_iniciales = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-a") == 0) {
            alg_reemplazo = argv[++i];
        } else if (strcmp(argv[i], "-f") == 0) {
            archivo_referencias = argv[++i];
        }
    }

    if (num_marcos_iniciales <= 0 || alg_reemplazo.empty() || archivo_referencias.empty()) {
        std::cerr << "Uso incorrecto: " << argv[0] << " -m <num-marcos-iniciales> -a <alg-reemplazo> -f <archivo-referencias>" << std::endl;
        return EXIT_FAILURE;
    }

    // Leer referencias desde archivo
    std::ifstream file(archivo_referencias);
    if (!file.is_open()) {
        std::cerr << "Error al abrir archivo de referencias.\n";
        return EXIT_FAILURE;
    }

    std::vector<int> referencias;
    int ref;
    while (file >> ref) {
        referencias.push_back(ref);
    }
    file.close();

    int fallos_pagina = 0;

    // Seleccionar algoritmo
    if (alg_reemplazo == "FIFO") {
        fallos_pagina = simular_fifo(referencias, num_marcos_iniciales);
    } else if (alg_reemplazo == "LRU") {
        fallos_pagina = simular_lru(referencias, num_marcos_iniciales);
    } else if (alg_reemplazo == "OPT") {
        fallos_pagina = simular_opt(referencias, num_marcos_iniciales);
    } else if (alg_reemplazo == "CLOCK") {
        fallos_pagina = simular_clock(referencias, num_marcos_iniciales);
    } else {
        std::cerr << "Algoritmo no reconocido: " << alg_reemplazo << "\n";
        return EXIT_FAILURE;
    }

    std::cout << "Fallos de página: " << fallos_pagina << std::endl;
    return EXIT_SUCCESS;
}
