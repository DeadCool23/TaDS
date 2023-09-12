#include <stdio.h>
#include "nums.h"
#include "errs.h"
#include "helper.h"

int main(void) {
    err_t err = OK;
    program_info();

    /// Ввод целого длинного числа
    lllong_t int_mult;
    input_helper('i');
    err = integer_input(&int_mult);
    if (err) goto ENDING;
    printf("\n\n");

    /// Ввод экспоненциального длинного числа
    lexp_t exp_mult;
    input_helper('f');
    err = exponent_input(&exp_mult);
    if (err) goto ENDING;
    printf("\n\n");

    /// Подсчет результата
    llexp_t mult_res;
    err = multiply(&int_mult, &exp_mult, &mult_res);
    if (err) goto ENDING;

    /// Форматный вывод посчитанного значения
    err = formated_output(&mult_res);
    if (err) goto ENDING;

    ENDING:
    if(err) err_message(err);
    return err;
}
