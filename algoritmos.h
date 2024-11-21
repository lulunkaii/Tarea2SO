#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>

/**
 * Simula el reemplazo de páginas utilizando el algoritmo FIFO (First-In, First-Out).
 *
 * @param references Vector de referencias de páginas virtuales.
 * @param num_frames Número de marcos de página disponibles.
 * @return Número de fallos de página.
 */
int simulate_fifo(const std::vector<int>& references, int num_frames);

/**
 * Simula el reemplazo de páginas utilizando el algoritmo LRU (Least Recently Used).
 *
 * @param references Vector de referencias de páginas virtuales.
 * @param num_frames Número de marcos de página disponibles.
 * @return Número de fallos de página.
 */
int simulate_lru(const std::vector<int>& references, int num_frames);

/**
 * Simula el reemplazo de páginas utilizando el algoritmo Óptimo.
 *
 * @param references Vector de referencias de páginas virtuales.
 * @param num_frames Número de marcos de página disponibles.
 * @return Número de fallos de página.
 */
int simulate_opt(const std::vector<int>& references, int num_frames);

/**
 * Simula el reemplazo de páginas utilizando el algoritmo de reloj (Clock).
 *
 * @param references Vector de referencias de páginas virtuales.
 * @param num_frames Número de marcos de página disponibles.
 * @return Número de fallos de página.
 */
int simulate_clock(const std::vector<int>& references, int num_frames);

#endif // ALGORITHMS_H
