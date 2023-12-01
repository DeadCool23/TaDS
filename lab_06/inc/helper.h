#ifndef __HELPER_H__
#define __HELPER_H__

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

/** @brief Процедура очистки буфера */
void clean_buf(void);
/**
 * @brief Функция проверки конда файла
 * @details Обрабатывает /n на конце файла
 * 
 * @param [in, out] file - файловая переменная
 * @return true - конец файла
 * @return false - не конец файла
 */
bool is_eof(FILE *file);
/**
 * @brief Функция получения микросекунд
 * @details Инспользуется gettimeofday()
 * 
 * @return время в микросекундах
 */
ssize_t microseconds_now(void);

#endif //__HELPER_H__
