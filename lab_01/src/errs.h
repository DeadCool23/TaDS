#ifndef __ERRS_H__
#define __ERRS_H__

/// @enum err_t - коды ошибок
typedef enum {
    OK, /// Все хорошо
    ERR_IO, /// Ошибка ввода
    ERR_RANGE, /// Ошибка диапазона
    ERR_CALC_RANGE /// Ошибка диапазона посчитанного числа
} err_t;

/**
 * @brief Процедура вывода сообщения об ошибках
 * @param err - Код ошибки
 */
void err_message(err_t err);

#endif //__ERRS_H__
