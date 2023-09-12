#include "../inc/helper.h"
#include <stdio.h>

void program_info(void) {
    printf("\033[0;32m-------------------------------------------------------------------------------------------------------\n\033[0m");
    printf("\033[0;32m|                                     Multiply of long numbers                                        |\n\033[0m");
    printf("\033[0;32m-------------------------------------------------------------------------------------------------------\n\033[0m");
    printf("|                          First number is integer. \033[4mFormat: [+/-]?[0-9]*\033[0m                              |\n");
    printf("|                 First number is exponential. \033[4mFormat: [+-]?[0-9]*([Ee][+-]?[0-9])?\033[0m                   |\n");
    printf("|                                  Result is exponential number                                       |\n");
    printf("|Result print faormat: [+-]0.(m_res)e(K_res), where m_res - mantiss of result, K_res - order of result|\n");
    printf("-------------------------------------------------------------------------------------------------------");
    printf("\n\n");
}

void input_helper(const char type) {
    switch (type)
    {
    /// Линейка целого числа
    case 'i':
        printf("                                       |->without sign\n");
        printf("                 integer               ||->with sign\n");
        printf(">--------1---------2---------3---------4|\n");
        break;
    /// Линейка экспоненциального числа
    case 'f':
        printf("                                  |->without signs\n");
        printf("          exponential num         ||->with one sign\n");
        printf("                                  |||->with two signs\n");
        printf(">--------1---------2---------m----|||\n");
        break;
    }
}
