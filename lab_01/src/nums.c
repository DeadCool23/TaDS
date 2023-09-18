#include "../inc/nums.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include <math.h>

bool is_sign(const char symb) { return symb == '+' || symb == '-'; }

err_t integer_input(lllong_t *num) {
    size_t i = 0;
    char symb = '\0';
    bool input_sign = false;    /// Вводился знак
    bool leading_nulls = false; /// Есть ли лидирующие нули

    num->sign = true;
    while ((symb = getchar()) != '\n') {
        if (is_sign(symb)) /* Ввод знака */ {
            /// Проверка единсвенности и правильности расположения знака
            if (!i && !input_sign) {
                num->sign = (symb == '+') ? true : false;
                input_sign = true;
            } else return ERR_IO;
        } else if (isdigit(symb)) /* Ввод цифр */ {
            if (!i && symb == '0') leading_nulls = true;     /// Откидывание лидирующих нулей
            else if (i < LLLONG_LEN) num->digits[i++] = symb; /// Запись цифр в структуру
            else return ERR_IO;
        } else return ERR_IO;
    }
    /// Ноль 
    if (leading_nulls && !i) {
        num->sign = true;
        num->digits[i++] = '0';
    }
    num->digits[i] = '\0';
#ifdef OUTPUT
    printf("%c%s\n", num->sign ? '+' : '-', num->digits);
#endif //OUTPUT
    /// Если число не было введено ошибка 
    return strlen(num->digits) ? OK : ERR_IO;
}

void exp_allign(void *num, const exp_allign_t allig_type, const exp_type_t type) {
    /* Поучение порядка и мантиссы */
    int *order;
    char *mantiss;
    if (type == LEXP) {
        mantiss = ((lexp_t *)num)->mantiss;
        order = &((lexp_t *)num)->num_order;
    } else if (type == LLEXP) {
        mantiss = ((llexp_t *)num)->mantiss;
        order = &((llexp_t *)num)->num_order;
    } else {
        mantiss = ((lllexp_t *)num)->mantiss;
        order = &((lllexp_t *)num)->num_order;       
    }
    
    ssize_t i;
    int add_to_order = 0;
    char tmp_mantiss[LLLEXP_LEN + 1];
    switch (allig_type) {
        /// Удаление лидирующих нулей
        case DEL_LEAD_NULLS:
            for (i = 0; mantiss[i] == '0'; i++);
            (size_t)i == strlen(mantiss) ? strcpy(mantiss, "0") : strcpy(mantiss, strcpy(tmp_mantiss, mantiss + i));
            return;
        /// Преобразование конечных нулей в порядок
        case END_NULLS_TO_EXP:
            for (i = strlen(mantiss) - 1; mantiss[i] == '0'; i--, add_to_order++);
            if (i >= 0) {
                mantiss[i + 1] = '\0';
                *order += add_to_order;
            } else {
                strcpy(mantiss, "0");
                *order = 0;
            }
            return;
    }
}


err_t exponent_input(lexp_t *num) {
    size_t i = 0;
    char symb = '\0';

    int cnt_unordered = 0;      /// Счетчик чисел после '.'
    bool exp_part = false;      /// Есть ли экспоненциальная часть
    bool is_matiss = false;     /// Введина ли мантисса
    bool input_sign = false;    /// Введен ли знак
    bool leading_nulls = false; /// Есть ли лидирующие нули

    num->sign = true;
    while ((symb = getchar()) != '\n') {
        if (is_sign(symb)) /* Ввод знака */ {
            /// Проверка единсвенности и правильности расположения знака
            if (!i && !input_sign) {
                num->sign = (symb == '+') ? true : false;
                input_sign = true;
            } else return ERR_IO;
        } else if (isdigit(symb) || symb == '.') {
            int float_nulls = 0;             /// Счетчик нулей почле точки
            bool with_float = false;         /// Есть ли дробная часть
            bool only_nulls_in_float = true; /// ТОлько ли нули в дробной части 
            do {
                /// Ввод дробной части
                if (symb == '.' && !with_float) with_float = true; 
                else if (isdigit(symb)) /*Ввод цифр*/{
                    if (!i && symb == '0') leading_nulls = true;  /// Откидывание лидирующих нулей
                    else if (i < LEXP_LEN) {
                        /// Проверка наличия ненулей в дробной части
                        if (with_float && symb != '0') only_nulls_in_float = false;
                        num->mantiss[i++] = symb;                                 /// Запись числа в мантиссу
                        float_nulls += only_nulls_in_float && with_float ? 1 : 0; /// Подсчет кол-ва нулей в дробной части
                    }
                    else return ERR_IO;
                    cnt_unordered += with_float ? 1 : 0; /// Подсчет дробных значений
                } else return ERR_IO;
                symb = getchar();
            } while(isdigit(symb) || symb == '.');
            /// Обнуление счетчика дробных значений если там одни нули
            if (only_nulls_in_float) cnt_unordered = 0; 
            /// Запись окончания числа в мантиссу
            num->mantiss[only_nulls_in_float && with_float ? i - (float_nulls - 1) : i] = '\0';
            is_matiss = strlen(num->mantiss) ? true : false;
            ungetc(symb, stdin); /// Возват последного забранного символа
        } else if (strchr("Ee", symb) && is_matiss) /** Обработка экспоненциальной части */ {
            /// Проверка символа после указание на наличие экспоненциальной части
            char check_exp = getchar();
            if (!is_sign(check_exp) && !isdigit(check_exp)) return ERR_IO;
            ungetc(check_exp, stdin);
            /// Считывание порядка
            int tmp_order;
            if (scanf("%d", &tmp_order) != 1) return ERR_IO;
            /// Проверка символов после считанного порядка
            char last_symb = getchar();
            if (last_symb != '\n') return ERR_IO;
            ungetc(last_symb, stdin);
            /// Проверка на разрешенный диапазон порядка экспоненциального числа
            if ((tmp_order < 0 ? -tmp_order : tmp_order) / (int)pow(10, ORDER_LEN) != 0) return ERR_IO;
            /// добавление доп порядка
            tmp_order -= cnt_unordered;
            num->num_order = tmp_order;
            exp_part = true;
        } else return ERR_IO;
    }
    /// Запись порядка в структуру
    num->num_order = exp_part ? num->num_order : -cnt_unordered;
    /// Ноль
    if (leading_nulls && !i) {
        num->sign = true;
        strcpy(num->mantiss, "0");
        num->num_order = 0;
    }
    /// Вравнивание нулей на конце мантиссы
    exp_allign(num, END_NULLS_TO_EXP, LEXP);
#ifdef OUTPUT
    printf("%c%se%d\n", num->sign ? '+' : '-', num->mantiss, num->num_order);
#endif //OUTPUT
    /// Если число не было введено ошибка 
    return is_matiss ? OK : ERR_IO;
}

err_t mult_on_digit(const char *num, const char char_digit, char *res) {
    uint8_t rest = 0;                      /// Остаток
    size_t num_len = strlen(num);          /// длина числа
    uint8_t mult_digit = char_digit - '0'; /// Цифра множитель
    char *res_ptr = res + LLLEXP_LEN;      /// Конец результирующей строки
    char *write_ptr = res_ptr;             /// Указатель на запись

    *res_ptr = '\0';

    for (ssize_t i = num_len - 1; i >= 0; i--) {
        uint8_t num_digit = num[i] - '0';                      /// Получиние цифры i-того разряда
        uint8_t sum_of_digits = num_digit * mult_digit + rest; /// Перемножение с добавлением остатка
        *(--write_ptr) = (sum_of_digits % 10) + '0';           /// Запись результата перемножения цифр
        rest = sum_of_digits / 10;                             /// Получение остатка
    }
    
    /// Запись остатка при наличии
    if (rest && write_ptr > res) *(--write_ptr) = rest + '0';
    else if (rest) return ERR_CALC_RANGE;

    /// Дозапись нулей в результируюзую строку
    while (write_ptr > res) *(--write_ptr) = '0';

    /// Возврат ошибки при переполнении
    return OK;
}

err_t sum_mult(const char *term_fst, const char *term_scd, char *res) {
    uint8_t rest = 0;                                        /// Остаток
    size_t len_fst = strlen(term_fst);                       /// Длина первого числа
    size_t len_scd = strlen(term_scd);                       /// Длина второго числа
    ssize_t max_len = len_fst > len_scd ? len_fst : len_scd; /// Максимальная длина числа
    char *res_ptr = res + LLLEXP_LEN;                        /// Конец результирующей строки
    char *write_ptr = res_ptr;                               /// Указатель на запись

    *res_ptr = '\0';

    ssize_t i = len_fst - 1;
    ssize_t j = len_scd - 1;

    for (; max_len > 0; max_len--) {
        uint8_t digit_fst = (i >= 0) ? term_fst[i--] - '0' : 0; /// Получение цифры первого числа
        uint8_t digit_scd = (j >= 0) ? term_scd[j--] - '0' : 0; /// Получение цифры второго числа

        uint8_t sum_of_digits = digit_fst + digit_scd + rest;   /// Сумма с учетом остатка
        *(--write_ptr) = (sum_of_digits % 10) + '0';            /// Запись полученной суммы
        rest = sum_of_digits / 10;                              /// Получение остатка
    }
    
    /// Запись остатка при наличии
    if (rest && write_ptr > res) *(--write_ptr) = rest + '0';
    else if (rest) return ERR_CALC_RANGE;

    /// Дозапись нулей в результируюзую строку
    while (write_ptr > res) *(--write_ptr) = '0';

    /// Возврат ошибки при переполнении
    return OK;
}

err_t multiply(const lllong_t *int_mult, const lexp_t *exp_mult, llexp_t *res) {
    lllexp_t tmp_mult;
    /// Знак результата
    tmp_mult.sign = !(int_mult->sign ^ exp_mult->sign);
    /// Порядок результата
    tmp_mult.num_order = exp_mult->num_order;

    bool add_null_to_end = false;
    char res_mantiss[LLLEXP_LEN + 1];  /// Мантисса результата
    char int_mult_num[LLLEXP_LEN + 1]; /// Целый множитель

    strcpy(int_mult_num, int_mult->digits);
    char sum_of_mult[LLLEXP_LEN + 1] = "0";     /// Сумма поцифренных произведений
    size_t exp_len = strlen(exp_mult->mantiss); /// Длина экспоненциального числа

    for (ssize_t i = exp_len - 1; i >= 0; i--) {
        char mult_on_i_digit[LLLEXP_LEN + 1];             /// Произведение числа на i-тую цифру экспоненциального числа
        strcat(int_mult_num, add_null_to_end ? "0" : ""); /// Поразрядное выравнивание

        /// Произведение числа на i-тую цифру экспоненциального числа
        err_t err = mult_on_digit(int_mult_num, exp_mult->mantiss[i], mult_on_i_digit);
        if (err) return err;

        /// Сложение полученного произведения с i-тым разрядом с суммой произведений числа с предыдущими разрядами
        err = sum_mult(sum_of_mult, mult_on_i_digit, res_mantiss);
        if (err) return err;

        strcpy(tmp_mult.mantiss, res_mantiss);
        strcpy(sum_of_mult, tmp_mult.mantiss);
        add_null_to_end = true; /// Указание на надобность выравнивани по разрядам
    }
    /// Запись результата в структуру ответа
    strcpy(tmp_mult.mantiss, sum_of_mult);
    /// Выравнивание мантиссы
    exp_allign(&tmp_mult, DEL_LEAD_NULLS, LLLEXP);
    exp_allign(&tmp_mult, END_NULLS_TO_EXP, LLLEXP);
#ifdef OUTPUT
    printf("%c%se%d\n", tmp_mult.sign ? '+' : '-', tmp_mult.mantiss, tmp_mult.num_order);
#endif //OUTPUT
    /// Проверка на надобность округления
    if (strlen(tmp_mult.mantiss) <= LLEXP_LEN)
        to_smaller_type(&tmp_mult, res);
    else {
        round_to_smaller(&tmp_mult);
        to_smaller_type(&tmp_mult, res);
    }
#ifdef OUTPUT
    printf("%c%se%d\n", res->sign ? '+' : '-', res->mantiss, res->num_order);
#endif //OUTPUT
    return OK;
}

void round_to_smaller(lllexp_t *to_small) {
    char round_mantiss[LLLEXP_LEN + 1]; /// Мантисса для записис округленного значения
    strcpy(round_mantiss, to_small->mantiss);
    char digit_to_round = *(round_mantiss + LLEXP_LEN); /// Получение невмещаемой в мантиссу цифры
    *(round_mantiss + LLEXP_LEN) = '\0';                /// Обрезание строки до допустимого размера мантиссы
    to_small->num_order += strlen(to_small->mantiss) - strlen(round_mantiss);
    if (digit_to_round - '0' < 5) /* Округление вниз */ {
        strcpy(to_small->mantiss, round_mantiss); 
    } else  /* Округление вверх */ {
        sum_mult(round_mantiss, "1", to_small->mantiss);
        exp_allign(to_small, DEL_LEAD_NULLS, LLLEXP);
        exp_allign(to_small, END_NULLS_TO_EXP, LLLEXP);
    }
}

void to_smaller_type(const lllexp_t *to_small, llexp_t *small) {
    small->sign = to_small->sign;
    small->num_order = to_small->num_order;
    strcpy(small->mantiss, to_small->mantiss);
}

err_t formated_output(const llexp_t *num) {
    int num_ord = !strcmp(num->mantiss, "0") /* Проверка нуля */ ? 0 : strlen(num->mantiss) + num->num_order; /// получение форматного порядка
    if ((num_ord < 0 ? -num_ord : num_ord) / (int)pow(10, ORDER_LEN) != 0) return ERR_CALC_RANGE;             /// Проверка выхода порядка за разрешенный размер
    printf("Result:\n");
    printf("%c0.%se%d\n", num->sign ? '+' : '-', num->mantiss, num_ord); /// Вывод числа
    return OK;
}
