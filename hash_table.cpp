#include "hash_table.h"
#include <list>
#include <iostream>

template <typename T>
hash_table_paginas<T>::hash_table_paginas(int size) {
    this->size = size;
    table = new std::list<int>[size];
}

template <typename T>
void hash_table_paginas<T>::insert(T key) {
    int index = key % size;
    table[index].push_back(key);
}

template <typename T>
void hash_table_paginas<T>::remove(T key) {
    int index = key % size;
    table[index].remove(key);
}

template <typename T>
bool hash_table_paginas<T>::search(T key) {
    int index = key % size;
    for (auto it = table[index].begin(); it != table[index].end(); it++) {
        if (*it == key) {
            return true;
        }
    }
    return false;
}

template <typename T>
hash_table_paginas<T>::~hash_table_paginas() {
    delete[] table;
}