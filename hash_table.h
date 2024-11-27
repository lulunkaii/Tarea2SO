#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <list>
#include <iostream>

template <typename T>
class hash_table_paginas {
    int size;
    std::list<T> *table;

public:
    hash_table_paginas(int size);

    void insert(T key);

    void remove(T key);

    bool search(T key);

    ~hash_table_paginas();
};

#endif // HASH_TABLE_H