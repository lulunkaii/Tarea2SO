#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include "algoritmos.h"
#include "hash_table.h"
#include "hash_table_pair.h"

int simular_fifo(const std::vector<int>& referencias, int numero_marcos) {
    return 0;
}

int simular_lru(const std::vector<int>& referencias, int numero_marcos) {
    return 0;
}

int simular_opt(const std::vector<int>& referencias, int numero_marcos) {
    return 0;
}

int simular_clock(const std::vector<int>& referencias, int numero_marcos) {
    hash_table_pair marcos(numero_marcos);
    int fallos = 0;

    for (int i = 0; i < numero_marcos; i++) {
        if (!marcos.search(referencias[i])) {
            fallos++;
            marcos.insert(referencias[i]);
        }
        

    }
    return fallos;
}
