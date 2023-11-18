#include "addressfrag.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct address_frag {
    size_t cnt;
    void *address;
    struct address_frag *next;
};

addresses_t *adr_init(void) { return NULL; }

err_t add_address(addresses_t **adrs, void *address) {
    bool added = false;
    addresses_t *prev = *adrs;
    for (addresses_t *iadr = *adrs; iadr != NULL && !added; iadr = iadr->next) {
        if (iadr->address == address) {
            iadr->cnt++;
            added = true;
        }
        prev = iadr;
    }
    
    if (!added) {
        addresses_t *new_address = malloc(sizeof(*new_address));
        if (!new_address)
            return ERR_MEM;
        new_address->address = address;
        new_address->cnt = 0;
        new_address->next = NULL;

        if (!prev) {
            new_address->next = *adrs;
            *adrs = new_address;
        } else {
            new_address->next = prev->next;
            prev->next = new_address;
        }
    }
    return OK;
}

void adrs_print(addresses_t *adrs, size_t adr_cnt) {
#define SEPARATOR printf("+-----+----------------+--------+\n")
    printf("\n\nINFO:\n");
    printf("\033[0;33mYellow address\033[0m - address used more than once\n");
    printf("White address - address used once\n");
    
    printf("\nTABLE:\n");
    SEPARATOR;
    printf("|%5s|%16s|%8s|\n", "i", "address", "Used cnt");
    SEPARATOR;
    
    addresses_t *iadr = adrs;
    for (size_t i = 0; i < adr_cnt && iadr != NULL; i++, iadr = iadr->next) {
        printf("|%5zu|", i + 1);
        printf((!iadr->cnt) ? " %14p " : " \033[0;33m%14p\033[0m ",  iadr->address);
        printf("|%8zu|\n", iadr->cnt + 1);
    }

    if (adr_cnt != 0) SEPARATOR;
    if (iadr) {
        printf("| ...                           |\n");
        printf("+-------------------------------+\n");
    }
    printf("\n\n");
#undef SEPARATOR
}

void adr_free(addresses_t *adrs) {
    for (addresses_t *iadr = adrs; iadr != NULL; ) {
        addresses_t *tmp_adr = iadr->next;
        free(iadr);
        iadr = tmp_adr;
    }
}
