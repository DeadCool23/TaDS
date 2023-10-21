#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <unistd.h>
#include <stddef.h>

/// @typedef структура матрицы
typedef struct {
    size_t rows; // Кол-во строк
    size_t cols; // Кол-во столбцов
    int **data;  // Элементты матрицы
} matrix_t;

/// @typedef структура вектора-строки
typedef struct {
    size_t size;   // Кол-во столбцов
    int *vector;  // Вектор-строка
} rvector_t;

/// @typedef структура разреженной матрицы
typedef struct {
    size_t rows;      // Кол-во строк
    size_t cols;      // Кол-во столбцов
    size_t els_cnt;   // Кол-во ненулевых элементов 
    int *els;         // Элементы матрицы
    size_t *el_i;     // Номера строк элементов
    ssize_t  *el_j;   // Индекс первого ненулевого элемента в столбце
} msparce_t;

/// @typedef структура разреженного вектора-строки
typedef struct {
    size_t size;    // Кол-во столбцов
    size_t els_cnt; // Кол-во ненулевых элементов
    int *els;       // Элементы вектора-строки
    size_t *el_j;   // Номера столбцов элементов
} rvsparce_t;

#endif //__MATRIX_H__

