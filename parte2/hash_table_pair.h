#ifndef HASH_TABLE_PAIR
#define HASH_TABLE_PAIR

#include <list>
#include <vector>
#include <utility>

class hash_table_pair {
    int size_inicial;
    int num_elements;
    std::list<std::pair<int,bool>> *table;
    std::vector<int> pages; // Vector para mantener el orden de las páginas
    int clock_hand; // Índice para el puntero del reloj

public:
    hash_table_pair(int size);

    void insert(int key);

    void remove(int key);

    bool search(int key);

    int size() const;

    ~hash_table_pair();
};

#endif // HASH_TABLE_H