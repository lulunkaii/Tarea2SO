#include "hash_table_pair.h"
#include <list>
#include <vector>
#include <utility>

hash_table_pair::hash_table_pair(int size) {
    this->size_inicial = size;
    this->num_elements = 0;
    this->clock_hand = 0;
    table = new std::list<std::pair<int, bool>>[size];
}

void hash_table_pair::insert(int key) {
    int index = key % size_inicial;

    // Buscar si la página ya está en la tabla
    for (auto& pair : table[index]) {
        if (pair.first == key) {
            pair.second = true; // Actualizar el bit de referencia
            return;
        }
    }

    // Si la página no está en la tabla y hay espacio
    if (num_elements < size_inicial) {
        table[index].push_back(std::make_pair(key, true));
        pages.push_back(key);
        num_elements++;
    } else {
        // Algoritmo Clock para encontrar una página para reemplazar
        while (true) {
            int current_page = pages[clock_hand];
            int current_index = current_page % size_inicial;

            for (auto it = table[current_index].begin(); it != table[current_index].end(); ++it) {
                if (it->first == current_page) {
                    if (it->second == false) {
                        table[current_index].erase(it);
                        table[index].push_back(std::make_pair(key, true));
                        pages[clock_hand] = key;
                        return;
                    } else {
                        it->second = false;
                    }
                    break;
                }
            }
            clock_hand = (clock_hand + 1) % size_inicial;
        }
    }
}

void hash_table_pair::remove(int key) {
    int index = key % size_inicial;
    for (auto it = table[index].begin(); it != table[index].end(); ++it) {
        if (it->first == key) {
            table[index].erase(it);
            num_elements--;
            break;
        }
    }
}

bool hash_table_pair::search(int key) {
    int index = key % size_inicial;
    for (const auto& pair : table[index]) {
        if (pair.first == key) {
            return true;
        }
    }
    return false;
}

int hash_table_pair::size() const {
    return num_elements;
}

hash_table_pair::~hash_table_pair() {
    delete[] table;
}