#ifndef __THEATRE_H__
#define __THEATRE_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

//----------------------------
#define MAX_THEATRE_NAME 30    /// @def MAX_THEATRE_NAME - максимальная длина названия театра
#define MAX_PERFOMACE_NAME 30  /// @def MAX_PERFOMACE_NAME - максимальная длина названия представления
#define MAX_COMPOSER_NAME 30   /// @def MAX_COMPOSER_NAME - максимальная длина имени композитора
#define MAX_COUNTRY_NAME 30    /// @def MAX_COUNTRY_NAME - максимальная длина названия страны
#define RANGE_PRICE 2          /// @def RANGE_PRICE - длина массива диапозона цен
//----------------------------

// @enum play_type_t - перечисление типов спектаклей
typedef enum { 
    PIESA,    // Пьеса
    DRAMA,    // Драма
    COMEDY,   // Комедия
    FAIRYTALE // Сказка
} play_type_t;

// @enum musicaly_type_t - перечисление типов музыкальных представлений
typedef enum { 
    BALLET,   // Балет
    OPERA,    // Опера
    MUSICAL   // Мюзикл
} musicaly_type_t;

// @struct play_t - структура спектакля
typedef struct {
    play_type_t type; // Тип спектакля
    bool for_kids;    // Детский ли спектаклб
    uint16_t age;     // Проходной возраст спектакля
} play_t;

// @struct musicaly_t - структура музыкального представления
typedef struct {
    char composer_name[MAX_COMPOSER_NAME + 1]; // Имя композитора
    char country_name[MAX_COUNTRY_NAME + 1];   // Страна
    musicaly_type_t type;                      // Тип музыкального представления
    uint16_t age;                              // Проходной возраст 
    uint32_t duration;                         // Продолжительность
} musicaly_t;

// @union perfomance_t - представление
typedef union {
    play_t play;         // Структура спектакля
    musicaly_t musical;  // Структура музыкального представления
} perfomance_t;

// @enum perfomance_type_t - типы представлений
typedef enum { 
    PLAY,     // Спектакль
    MUSICALY  // Музыкальное представление
} perfomance_type_t;

// @struct theatre_t - структура репертура театра
typedef struct {
    char theatre_name[MAX_THEATRE_NAME + 1];      // Название театра
    char perfomance_name[MAX_PERFOMACE_NAME + 1]; // Название представления
    uint32_t price_range[RANGE_PRICE];            // Диапазон цены 
    perfomance_type_t type;                       // Тип представления
    perfomance_t perfomance;                      // Структура представления
} theatre_t;

// @struct theatre_table_t - такблица репертуаров театра
typedef struct {
    size_t size;          // Размер
    theatre_t *theatres;  // Массив репертуаров структур
} theatre_table_t;

// @struct key_t - структура ключа
typedef struct {
    char theatre_name[MAX_THEATRE_NAME + 1]; // Название театра
    size_t ind;                              // Индекс в таблице репертуаров театров
} key_t;

// @struct key_table_t - таблица ключей
typedef struct keys_table{
    key_t *keys;                                                                  // Массив ключей
    size_t size;                                                                  // Размер
    void (*key_table_gen)(struct keys_table (*) ,theatre_table_t);                // Генератор таблицы ключей
    void (*free_key_table)(struct keys_table (*));                                // Освобождение таблицы ключей
} key_table_t;

#endif  //__THEATRE_H__
