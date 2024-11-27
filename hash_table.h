#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <list>
#include <iostream>

template <typename T>
class hash_table_paginas {
    int size_inicial;
    int num_elements;
    std::list<T> *table;

public:
    hash_table_paginas(int size_inicial);

    void insert(T key);

    void remove(T key);

    bool search(T key);
    
    int size();

    ~hash_table_paginas();
};

#endif // HASH_TABLE_H