#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <cstring>
#include "algoritmos.h"

using namespace std;

// Prototipos de funciones
int simulate_fifo(const vector<int>& references, int num_frames);
int simulate_lru(const vector<int>& references, int num_frames);
int simulate_opt(const vector<int>& references, int num_frames);
int simulate_clock(const vector<int>& references, int num_frames);

void print_usage() {
    cout << "Uso: ./mvirtual -m <num_marcos> -a <algoritmo> -f <archivo_referencias>\n";
    cout << "Algoritmos disponibles: OPT, FIFO, LRU, CLOCK\n";
}

int main(int argc, char* argv[]) {
    if (argc != 7) {
        print_usage();
        return EXIT_FAILURE;
    }

    int num_frames = 0;
    string algorithm, ref_file;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-m") == 0) {
            num_frames = stoi(argv[++i]);
        } else if (strcmp(argv[i], "-a") == 0) {
            algorithm = argv[++i];
        } else if (strcmp(argv[i], "-f") == 0) {
            ref_file = argv[++i];
        }
    }

    if (num_frames <= 0 || algorithm.empty() || ref_file.empty()) {
        print_usage();
        return EXIT_FAILURE;
    }

    // Leer referencias desde archivo
    ifstream file(ref_file);
    if (!file.is_open()) {
        cerr << "Error al abrir archivo de referencias.\n";
        return EXIT_FAILURE;
    }

    vector<int> references;
    int ref;
    while (file >> ref) {
        references.push_back(ref);
    }
    file.close();

    int page_faults = 0;

    // Seleccionar algoritmo
    if (algorithm == "FIFO") {
        page_faults = simulate_fifo(references, num_frames);
    } else if (algorithm == "LRU") {
        page_faults = simulate_lru(references, num_frames);
    } else if (algorithm == "OPT") {
        page_faults = simulate_opt(references, num_frames);
    } else if (algorithm == "CLOCK") {
        page_faults = simulate_clock(references, num_frames);
    } else {
        cerr << "Algoritmo no reconocido: " << algorithm << "\n";
        return EXIT_FAILURE;
    }

    cout << "Fallos de página: " << page_faults << endl;
    return EXIT_SUCCESS;
}
