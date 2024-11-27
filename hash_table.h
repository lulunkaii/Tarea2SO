#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <list>
#include <iostream>

class hash_table_paginas {
    int size;
    std::list<int> *table;

    hash_table_paginas(int size);

    void insert(int key);

    void remove(int key);

    bool search(int key);

    ~hash_table_paginas();
};

#endif // HASH_TABLE_H