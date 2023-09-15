#ifndef __NUMS_H__
#define __NUMS_H__

#include <stdbool.h>

#include "errs.h"

#define LLONG_LEN 40                          /// @def LLONG_LEN - Максимальный размер целого числа
#define LEXP_LEN 30                           /// @def LEXP_LEN - Максимальный размер мантиссы коротекого экспоненциального числа
#define LLEXP_LEN 40                          /// @def LLEXP_LEN - Максимальный размер мантиссы длинного экспоненциального числа
#define LLLEXP_LEN (LLONG_LEN + LEXP_LEN + 2) /// @def LLLEXP_LEN Максимальный размер матсиссы максимального экспоненциального числа
#define ORDER_LEN 5                           /// @def ORDER_LEN - Максимальный порядок экспоненциальных чисел

/// @struct lexp_t - экспоненциальное число с мантиссой размера 30
typedef struct {
    bool sign;
    char mantiss[LEXP_LEN + 1];
    int num_order;
} lexp_t;

/// @struct llexp_t - экспоненциальное число с мантиссой размера 40
typedef struct {
    bool sign;
    char mantiss[LLEXP_LEN + 1];
    int num_order;
} llexp_t;

/// @struct lllexp_t - экспоненциальное число с мантиссой размера 40
typedef struct {
    bool sign;
    char mantiss[LLLEXP_LEN + 1];
    int num_order;
} lllexp_t;


/// @struct lllong_t - целое число размера 40
typedef struct {
    bool sign;
    char digits[LLONG_LEN + 1];
} lllong_t;


/// @enum allign_t - выравнивание экспоненциальных чисел
typedef enum {
    DEL_LEAD_NULLS,   /// Выравнивание лидирующих нулей
    END_NULLS_TO_EXP, /// Выравнивание нулей на конце
} exp_allign_t;

/// @enum exp_type_t - типы экспоненциальных чисел
typedef enum {
    LEXP,   /// тип lexp_t
    LLEXP,  /// тип llexp_t
    LLLEXP, /// тип lllexp_t
} exp_type_t;

/**
 * @brief Функция проверки знака числа
 * 
 * @param [in] symb - поверяемый символ
 * @return true - если знак
 * @return false - если не знак
 */
bool is_sign(const char symb);

/**
 * @brief Функция ввода целого длинного числа
 * 
 * @param [out] num - вводмое число 
 * @return Код ошибки
 */
err_t integer_input(lllong_t *num);

/**
 * @brief Функция выравнивания экспоненциального числа
 * 
 * @param [in, out] num - Экспоненциальное числа
 * @param [in] allig_type - Тип выравнивания
 * @param [in] type - Тип экспоненциального числа
 */
void exp_allign(void *num, const exp_allign_t allig_type, const exp_type_t type);


/**
 * @brief Функция ввода экспоненциального длинного числа
 * 
 * @param [out] num - вводмое число 
 * @return Код ошибки
 */
err_t exponent_input(lexp_t *num);


/**
 * @brief Функция сложения чисел
 * 
 * @param [in] term_fst - первое число
 * @param [in] term_scd - второе число
 * @param [out] res - результирующее число
 * @return Код ошибки 
 */
err_t sum_mult(const char *term_fst, const char *term_scd, char *res);
/**
 * @brief Функция умножения числа на цифру
 * 
 * @param [in] num - число
 * @param [in] char_digit - цифра 
 * @param [out] res - результирующее число 
 * @return Код ошибки 
 */
err_t mult_on_digit(const char *num, const char char_digit, char *res);
/**
 * @brief 
 * 
 * @param [in] int_mult 
 * @param [in] exp_mult 
 * @param [out] res 
 * @return Код ошибки 
 */
err_t multiply(const lllong_t *int_mult, const lexp_t *exp_mult, llexp_t *res);

/**
 * @brief Функция округления большего типа в меньший
 * 
 * @param [in, out] to_small - больший тип
 */
void round_to_smaller(lllexp_t *to_small);
/**
 * @brief Функция записи большего типа в меньший
 * 
 * @param [in] to_small - больший тип
 * @param [out] small - меньший тип 
 */
void to_smaller_type(const lllexp_t *to_small, llexp_t *small);

/**
 * @brief Форматный вывод резкльтата
 * 
 * @param [in] num - выводимое число
 * @return Код ошибки
 */
err_t formated_output(const llexp_t *num);

#endif //__NUMS_H__
