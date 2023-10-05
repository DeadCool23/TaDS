#include "tableads.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static bool add_type(char type, theatre_t *theatre) {
    int perf_type = 0;
    
    switch (type) {
        case 'B':
            perf_type = -1;
            theatre->perfomance.musical.type = BALLET;
            break;
        case 'O':
            perf_type = -1;
            theatre->perfomance.musical.type = OPERA;
            break;
        case 'M':
            perf_type = -1;
            theatre->perfomance.musical.type = MUSICAL;
            break;

        case 'P':
            perf_type = 1;
            theatre->perfomance.play.type = PIESA;
            theatre->perfomance.play.for_kids = false;
            break;
        case 'D':
            perf_type = 1;
            theatre->perfomance.play.type = DRAMA;
            theatre->perfomance.play.for_kids = false;
            break;
        case 'C':
            perf_type = 1;
            theatre->perfomance.play.type = COMEDY;
            theatre->perfomance.play.for_kids = false;
            break;
        case 'F':
            perf_type = 1;
            theatre->perfomance.play.type = FAIRYTALE;
            theatre->perfomance.play.for_kids = true;
            break;
        default:
            return false;
    }

    switch (perf_type) {
        case -1:
            theatre->type = MUSICALY;
            break;
        case 1:
            theatre->type = PLAY;
            break;
    }
    return true;
}

static int musicaly_read(theatre_t *theatre) {
    printf("Input \033[4mComposer\033[0m name: ");
    FGETS_CHECKER(theatre->perfomance.musical.composer_name, MAX_COMPOSER_NAME, stdin);

    printf("Input \033[4mCountry\033[0m name: ");
    FGETS_CHECKER(theatre->perfomance.musical.country_name, MAX_COUNTRY_NAME, stdin);

    char tmp; int age;
    printf("Input \033[4mAge limit\033[0m\033[3m [Format: age+]\033[0m: ");
    SCANF_CHECKER(2, "%d%c", &age, &tmp);
    if (age < 0 || tmp != '+') return ERR_IO;
    theatre->perfomance.musical.age = age;

    int duration;
    printf("Input \033[4mDuration\033[0m of perfomance in minutes: ");
    SCANF_CHECKER(2, "%d%c", &duration, &tmp);
    if (duration <= 0) return ERR_IO;
    theatre->perfomance.musical.duration = duration;

    return OK;
}

err_t add_to_table(theatre_table_t *table) {
    theatre_t tmp;

    printf("Input \033[4mTheatre\033[0m name: ");
    FGETS_CHECKER(tmp.theatre_name, MAX_THEATRE_NAME, stdin);
    
    printf("Input \033[4mPerfomance\033[0m name: ");
    FGETS_CHECKER(tmp.perfomance_name, MAX_PERFOMACE_NAME, stdin);

    int min, max;
    printf("Input \033[4mPrice range\033[0m\033[3m [Format: min-max]\033[0m: ");
    SCANF_CHECKER(2, "%d-%d", &min, &max);
    if (min < 0 || max < 0 || min > max) return ERR_IO;
    tmp.price_range[0] = min;
    tmp.price_range[1] = max;

    char type;
    clean_buf();
    printf("Input \033[4mPerfomance type\033[0m\033[3m [B:Ballet|C:Comedy|D:Drama|F:Fairytale|M:Musical|O:Opera|P:Piesa]\033[0m: ");
    SCANF_CHECKER(1, "%c", &type);
    clean_buf();
    if (!add_type(type, &tmp)) return ERR_IO;

    if (tmp.type == MUSICALY) musicaly_read(&tmp);
    else if (tmp.perfomance.play.for_kids) {
        char format;
        int age;
        printf("Input \033[4mAge limit\033[0m\033[3m [Format: age+]\033[0m: ");
        SCANF_CHECKER(2, "%d%c", &age, &format);
        if (age < 0 || format != '+') return ERR_IO;
        tmp.perfomance.play.age = age;
    }
    theatre_t *p_tmp = (theatre_t *)realloc(table->theatres, (table->size + 1) * sizeof(*table->theatres));
    if (!p_tmp) return ERR_MEM;
    table->theatres = p_tmp;
    table->theatres[table->size++] = tmp;
    printf("\n");
    return OK;
}

err_t del_theatres_by_name(theatre_table_t *table) {
    char del_theatre[MAX_THEATRE_NAME + 1];
    theatre_table_t new_table;
    new_table.size = 0;
    printf("Input \033[4mTheatre\033[0m name for \033[0;31mdelete\033[0m: ");
    FGETS_CHECKER(del_theatre, MAX_THEATRE_NAME, stdin);

    for (size_t i = 0; i < table->size; i++) {
        if (strcmp(del_theatre, table->theatres[i].theatre_name) != 0) {
            theatre_t *p_tmp = realloc(new_table.theatres, (new_table.size + 1) * sizeof(*new_table.theatres));
            if (!p_tmp) return ERR_MEM;
            new_table.theatres = p_tmp;
            new_table.theatres[new_table.size++] = table->theatres[i];
        }
    }

    free(table->theatres);
    table->theatres = new_table.theatres;
    table->size = new_table.size;
    return OK;
}

err_t search_ballet_by_fiealds(theatre_table_t *table) {
    int age;
    printf("Input required \033[4mAge\033[0m: ");
    SCANF_CHECKER(1, "%d", &age);
    if (age < 0) return ERR_IO;
    int duration;
    printf("Input max \033[4mDuration\033[0m of perfomance in minutes: ");
    SCANF_CHECKER(1, "%d", &duration);
    if (duration <= 0) return ERR_IO;
    clean_buf();

    size_t count_ballets = 0;
    size_t *ballets_index = malloc(0);
    for (size_t i = 0; i < table->size; i++) {
        if (table->theatres[i].type == MUSICALY &&
        table->theatres[i].perfomance.musical.type == BALLET &&
        table->theatres[i].perfomance.musical.age < (uint16_t)age &&
        table->theatres[i].perfomance.musical.duration < (uint32_t)duration) {
            size_t *p_tmp = realloc(ballets_index, (count_ballets + 1) * sizeof(*ballets_index));
            if (!p_tmp) return ERR_MEM;
            ballets_index = p_tmp;
            *(ballets_index + count_ballets++) = i;
        }
    }

    if (!count_ballets) {
        printf("\n\n\033[4mNo matching ballets found\033[0m\n\n");
    } else {
        header('t');
        for (size_t i = 0; i < count_ballets; i++)
            theatre_print(&table->theatres[ballets_index[i]]);
        ending('t');
    }

    return OK;
}

void free_table(theatre_table_t *table) {
    free(table->theatres);
    table->theatres = NULL;
    table->size = 0;
}

