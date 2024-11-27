#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <list>
#include <iostream>

class hash_table_paginas {
    int size_inicial;
    int num_elements;
    std::list<int> *table;

public:
    hash_table_paginas(int size_inicial);

    void insert(int key);

    void remove(int key);

    bool search(int key);
    
    int size();

    ~hash_table_paginas();
};

#endif // HASH_TABLE_H