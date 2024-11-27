#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include "algoritmos.h"
#include "hash_table.h"
#include "hash_table_pair.h"

int simular_fifo(const std::vector<int>& referencias, int numero_marcos) {
    hash_table_paginas hash_table(numero_marcos);
    std::queue<int> cola;
    int fallos_pagina = 0;

    for (int i = 0; i < referencias.size(); i++) {
        int ref = referencias[i];
        if (!hash_table.search(ref)) {
            fallos_pagina++;
            if (cola.size() == numero_marcos) {
                int pagina_eliminada = cola.front();
                cola.pop();
                hash_table.remove(pagina_eliminada);
            }
            cola.push(ref);
            hash_table.insert(ref);
        }
    }
    return fallos_pagina;
}

int simular_lru(const std::vector<int>& referencias, int numero_marcos) {
    return 0;
}

int encontrar_reemplazo_opt(const std::vector<int>& referencias, int indice) {
    int pagina_a_remover = -1;
    int max_distancia = -1;

    std::vector<std::pair<int,int>> distancias;
    std::vector<int> referencias_distintas;

    for (int referencia : referencias) {
        if(std::find(referencias_distintas.begin(), referencias_distintas.end(), referencia) == referencias_distintas.end()) {
            referencias_distintas.push_back(referencia);
        }
    }

    for(int i = 0; i < referencias_distintas.size(); i++) {
        int distancia = 0;
        for(int j = indice; j < referencias.size(); j++) {
            distancia++;
            if(referencias[j] == referencias_distintas[i]) {
                break;
            }

        }
        distancias.push_back({referencias_distintas[i], distancia});
    }

    for(auto par: distancias) {
        if(par.second >= max_distancia) {
            max_distancia = par.second;
            pagina_a_remover = par.first;
        }
    }

    return pagina_a_remover;
}

int simular_opt(const std::vector<int>& referencias, int numero_marcos) {
    hash_table_paginas marcos(numero_marcos);
    int fallos_pagina = 0;
    int ultimo_agregado;

    for(int i = 0; i < referencias.size(); i++) {
        if(!marcos.search(referencias[i])) {
            fallos_pagina++;
            if(marcos.size() == numero_marcos) {
                int pagina_a_remover = encontrar_reemplazo_opt(referencias, i);
                if(marcos.search(pagina_a_remover)) {
                    marcos.remove(pagina_a_remover);
                } else {
                    pagina_a_remover = ultimo_agregado;
                    marcos.remove(pagina_a_remover);
                }
            }
            marcos.insert(referencias[i]);
            ultimo_agregado = referencias[i];
        } 
    }

    return fallos_pagina;
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
