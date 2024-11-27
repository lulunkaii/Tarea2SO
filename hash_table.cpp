#include "hash_table.h"
#include <list>
#include <iostream>

hash_table_paginas::hash_table_paginas(int size) {
    this->size_inicial = size;
    this->num_elements = 0;
    table = new std::list<int>[size];
}

void hash_table_paginas::insert(int key) {
    int index = key % size_inicial;
    this->num_elements++;
    table[index].push_back(key);
}

void hash_table_paginas::remove(int key) {
    int index = key % size_inicial;
    this->num_elements--;
    table[index].remove(key);
}

bool hash_table_paginas::search(int key) {
    int index = key % size_inicial;
    for (auto it = table[index].begin(); it != table[index].end(); it++) {
        if (*it == key) {
            return true;
        }
    }
    return false;
}

hash_table_paginas::~hash_table_paginas() {
    delete[] table;
}

int hash_table_paginas::size() {
    return this->num_elements;
}