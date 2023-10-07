#include "theatreio.h"

#include "inttypes.h"

#include "helper.h"

#include <stdio.h>
#include <stdlib.h>

#define SHORT_ALLIGN 10

static char *age_format(uint16_t age, char *form_age) {
    snprintf(form_age, SHORT_ALLIGN, "%" PRIu16 "+", age);
    return form_age;
}

static char *time_format(uint32_t minuntes, char *form_time) {
    snprintf(form_time, SHORT_ALLIGN, "%d:%02d", (int)minuntes / 60, (int)minuntes % 60);
    return form_time;
}

static void musicaly_type_print(musicaly_type_t type) {
    switch (type) {
    case BALLET:
        printf("%-20s", "Ballet");
        break;
    case OPERA:
        printf("%-20s", "Opera");
        break;
    case MUSICAL:
        printf("%-20s", "Musical");
        break;
    }
}

static void play_type_print(play_type_t type) {
    switch (type) {
    case PIESA:
        printf("%-20s", "Piesa");
        break;
    case DRAMA:
        printf("%-20s", "Drama");
        break;
    case COMEDY:
        printf("%-20s", "Comedy");
        break;
    case FAIRYTALE:
        printf("%-20s", "Fairytale");
        break;
    }
}

static void musicaly_print(const musicaly_t *musical) {
    musicaly_type_print(musical->type);
    char form_age[SHORT_ALLIGN];
    char form_direct[SHORT_ALLIGN];
    printf("|%-30s|%-30s|%-10s|%-10s|", musical->composer_name, 
    musical->country_name, age_format(musical->age, form_age), 
    time_format(musical->duration, form_direct));
}

static void play_print(const play_t *play) {
    play_type_print(play->type);
    char form_age[SHORT_ALLIGN];
    printf("|%-30s|%-30s|%-10s|%-10s|", EMPTY_FIELD, EMPTY_FIELD, 
    play->for_kids ? age_format(play->age, form_age) : EMPTY_FIELD, 
    EMPTY_FIELD);
}

void theatre_print(const theatre_t *theatre) {
#define PRICE_ALLIGN 27
    char price[PRICE_ALLIGN];
    snprintf(price, sizeof(price), "%" PRIu32 "—%" PRIu32 "", 
    theatre->price_range[0], theatre->price_range[1]);
    printf("|%-30s|%-30s|%-27s|", theatre->theatre_name, theatre->perfomance_name, price);
    switch (theatre->type)
    {
    case MUSICALY:
        musicaly_print(&theatre->perfomance.musical);
        break;
    case PLAY:
        play_print(&theatre->perfomance.play);
        break;
    }
    printf("\n");
}

//==========================================================================================

static bool is_real_type(char *type, theatre_t *theatre) {
    if (!strcmp(type, "Ballet")) {
        theatre->type = MUSICALY;
        theatre->perfomance.musical.type = BALLET; 
    } else if (!strcmp(type, "Opera")) {
        theatre->type = MUSICALY;
        theatre->perfomance.musical.type = OPERA; 
    } else if (!strcmp(type, "Musical")) {
        theatre->type = MUSICALY;
        theatre->perfomance.musical.type = MUSICAL; 
    } else if (!strcmp(type, "Piesa")) {
        theatre->type = PLAY;
        theatre->perfomance.play.type = PIESA;
        theatre->perfomance.play.for_kids = false;
    } else if (!strcmp(type, "Drama")) {
        theatre->type = PLAY;
        theatre->perfomance.play.type = DRAMA;
        theatre->perfomance.play.for_kids = false;
    } else if (!strcmp(type, "Comedy")) {
        theatre->type = PLAY;
        theatre->perfomance.play.type = COMEDY;
        theatre->perfomance.play.for_kids = false;
    } else if (!strcmp(type, "Fairytale")) {
        theatre->type = PLAY;
        theatre->perfomance.play.type = FAIRYTALE;
        theatre->perfomance.play.for_kids = true;
    } else return false;
    return true;
}

static err_t load_play_from_file(FILE *file, theatre_t *theatre) {
    err_t err = OK;
    char *tmp = NULL;
    tmp = get_str(file, '|', -1);
    if (!tmp || strcmp(tmp, "-")) { err = ERR_FILE; goto END; }
    free(tmp);

    tmp = get_str(file, '|', -1);
    if (!tmp || strcmp(tmp, "-")) { err = ERR_FILE; goto END; }
    free(tmp);

    if (theatre->perfomance.play.for_kids) {
        int age; char format;
        if (fscanf(file, "%d%c|", &age, &format) != 2) { err = ERR_FILE; goto END; }
        if (age < 0 || format != '+') { err = ERR_FILE; goto END; }
        theatre->perfomance.play.age = age;
    } else {
        tmp = get_str(file, '|', -1);
        if (!tmp || strcmp(tmp, "-")) { err = ERR_FILE; goto END; }
        free(tmp);
    }

    tmp = get_str(file, '|', -1);
    if (!tmp || strcmp(tmp, "-")) { err = ERR_FILE; goto END; }

    END:
    free(tmp);
    return err;
}

static err_t load_musical_from_file(FILE *file, theatre_t *theatre) {
    err_t err = OK;
    char *country = NULL;
    char *composer_name = NULL;
    composer_name = get_str(file, '|', MAX_COMPOSER_NAME);
    if (!composer_name) { err = ERR_FILE; goto END; }
    strcpy(theatre->perfomance.musical.composer_name, composer_name);

    country = get_str(file, '|', MAX_COMPOSER_NAME);
    if (!country) { err = ERR_FILE; goto END; }
    strcpy(theatre->perfomance.musical.country_name, country);

    int age; char format;
    if (fscanf(file, "%d%c|", &age, &format) != 2) { err = ERR_FILE; goto END; }
    if (age < 0 || format != '+') { err = ERR_FILE; goto END; }
    theatre->perfomance.musical.age = age;

    int duration;
    if (fscanf(file, "%d|", &duration) != 1) { err = ERR_FILE; goto END; }
    if (duration < 0 ) { err = ERR_FILE; goto END; }
    theatre->perfomance.musical.duration = duration;

    END:
    free(composer_name);
    free(country);
    return err;
}

err_t load_theatre_from_file(FILE *file, theatre_t *theatre) {
    err_t err = OK;
    char *theatre_name = NULL;
    char *perfomance_name = NULL;
    char *type = NULL;
    theatre_name = get_str(file, '|', MAX_THEATRE_NAME);
    if (!theatre_name) { err = ERR_FILE; goto END; }
    strcpy(theatre->theatre_name, theatre_name);

    perfomance_name = get_str(file, '|', MAX_PERFOMACE_NAME);
    if (!perfomance_name) { err = ERR_FILE; goto END; }
    strcpy(theatre->perfomance_name, perfomance_name);

    int min, max;
    if (fscanf(file, "%d-%d|", &min, &max) != 2) { err = ERR_FILE; goto END; }
    if (min < 0 || max < 0 || min > max) { err = ERR_FILE; goto END; }
    theatre->price_range[0] = min;
    theatre->price_range[1] = max;

    type = get_str(file, '|', -1);
    if (!type) { err = ERR_FILE; goto END; }
    if (!is_real_type(type, theatre)) { err = ERR_FILE; goto END; }
    switch (theatre->type)
    {
        case PLAY:
            err = load_play_from_file(file, theatre);
            break;
        
        case MUSICALY:
            err = load_musical_from_file(file, theatre);
            break;
    }
    END:
    free(perfomance_name);
    free(theatre_name);
    free(type);
    return err;
}

//==========================================================================================

static void load_musical_to_formated_file(FILE *file, const musicaly_t *musical) {
    fprintf(file, "%s|%s|%s|%" PRIu16 "+|%" PRIu32 "|", 
    (musical->type == BALLET) ? "Ballet" : (musical->type == OPERA) ? "Opera"
    : (musical->type == MUSICAL) ? "Musical" : "",
    musical->composer_name, musical->country_name, 
    musical->age, musical->duration);
}

static  void load_play_to_formated_file(FILE *file, const play_t *play) {
    fprintf(file, "%s|-|-|", (play->type == PIESA) ? "Piesa" : (play->type == COMEDY) ? "Comedy" 
    : (play->type == DRAMA) ? "Drama" : (play->type == FAIRYTALE) ? "Fairytale" : "");
    play->for_kids ? fprintf(file, "%" PRIu16 "+|-|", play->age) : fprintf(file, "-|-|");
}

void load_theatre_to_formated_file(FILE *file, const theatre_t *theatre) {
    fprintf(file, "%s|%s|%" PRIu32 "-%" PRIu32 "|", theatre->theatre_name, theatre->perfomance_name,
    theatre->price_range[0], theatre->price_range[1]);
    switch (theatre->type)
    {
        case PLAY:
            load_play_to_formated_file(file, &theatre->perfomance.play);
            break;
        
        case MUSICALY:
            load_musical_to_formated_file(file, &theatre->perfomance.musical);
            break;
    }
    fprintf(file, "\n");
}
