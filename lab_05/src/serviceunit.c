#include "serviceunit.h"

#include "mes.h"
#include "const.h"

#include <math.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#include "aqueue.h"
#include "lqueue.h"

#include "helper.h"

#define EPS 1e-11

#define FRAG_ADRESSES_PRINT(adresses)                         \
    do {                                                      \
        int cnt;                                              \
        char c, end;                                          \
        printf("Print free adresses?\n[Y: Yes; N: No]: ");    \
        SCANF_CHECKER(2, "%c%c", &c, &end);                   \
        if (end != '\n') return ERR_IO;                       \
        switch (c) {                                          \
            case 'Y':                                         \
                printf("Cnt of adresses: ");                  \
                SCANF_CHECKER(1, "%d", &cnt);                 \
                if (cnt < 0) return ERR_IO;                   \
                adrs_print(adresses, cnt);                    \
                break;                                        \
            case 'N':                                         \
                break;                                        \
            default:                                          \
                return ERR_CMD;                               \
        }                                                     \
    } while (0)

#define max(fst, scd) (fst) > (scd) ? (fst) : (scd)

#define is_T1_lower_T2 ((max(T1_RANGE(1)- T1_RANGE(0), T2_RANGE(1) - T2_RANGE(0))) == (T1_RANGE(1)- T1_RANGE(0)) ? true : false)

#define teor_modeling_time ((max(T1_RANGE(1)- T1_RANGE(0), T2_RANGE(1) - T2_RANGE(0))) / 2.0 * 1000 * (((fabs(CHANCE - 1.0) < EPS) ? (1 / (1 - CHANCE)) : 1)))

static double get_time(double beg, double end) { return beg + ((double)rand() / RAND_MAX) * (end - beg); }

/** @brief Обслуживающий аппарат */
static bool service_unit(void) { return ((rand() % 10) / 10.0 < CHANCE) ? false : true; }

static bool add_request(void) { return !(rand() % 2) ? false : true; }

static void print_queue_info(service_info_t *info, size_t cur_queue_len) {
    printf("\033[34mIssued requests:\033[0m %zu\n", info->out_requests_cnt);
    printf("Current queue len: %zu\n", cur_queue_len);
    printf("Average queue len: %.1lf\n", info->requests.sum / (double) info->requests.cnt);
    printf("\n\n");
}

static void print_full_service_unit_info(service_info_t *info) {
    printf("\033[3;34mINFO:\033[0m\n");
    printf("Expected modeling time: %.1lf\n", teor_modeling_time);
    printf("Recieved modeling time: %.1lf\n", info->modeling_time);
    printf("Margin: %.1lf%%\n", fabs(info->modeling_time - teor_modeling_time) / teor_modeling_time * 100);
    printf("\n");

    printf("Count of entered requests: %zu\n", info->in_requests_cnt);
    printf("Count of exited requests: %zu\n", info->out_requests_cnt);
    printf("Count of service triggerings: %zu\n", info->service_unit_triggers_cnt);
    printf("Simple time of service: %.1lf\n", info->simple_time);

    printf("Average time in queue : %.1lf\n", info->requests_in_queue.sum / info->requests_in_queue.cnt);
    printf("\n\n");
}

static service_info_t info_init(void) {
    return (service_info_t) {
        .simple_time = 0,
        .modeling_time = 0,
        .in_requests_cnt = 0,
        .out_requests_cnt = 0,
        .requests = {
            .cnt = 0,
            .sum = 0,
        },
        .requests_in_queue = {
            .cnt = 0,
            .sum = 0,
        },
        .service_unit_triggers_cnt = 0,
    };
}

static err_t arr_service_unit(addresses_t **adrs, bool get_mes, ...) {
    RESET_RAND;
    va_list is_mes;
    err_t err = OK;
    bool is_request_out = false;
    service_info_t su = info_init();
    aqueue_t queue; aq_init(&queue);

    va_start(is_mes, get_mes);

    mes_t *mes;
    if (get_mes) {
        mes = va_arg(is_mes, mes_t (*));    
        mes->mem = 0;
        mes->time = 0;
    }

    ssize_t beg = microseconds_now();
    if (CHANCE >= 1) {
        printf("Noone request from 1000 went out from queue\n");
        return ERR_CHANCE;
    }

    while (su.out_requests_cnt != REQUESTS_CNT) {
        double modeling_time = get_time(T1_RANGE(0), T1_RANGE(1));
        if (add_request()) {
            if ((err = aq_push(&queue, 0)))
                return err;
            su.in_requests_cnt++;
        }
        
        double request = aq_pop(&queue);
        if (request < 0) continue;
        request += (request < EPS) ? 0 : modeling_time;

        if (add_address(adrs, queue.first))
            return ERR_MEM;

        double simple_time = get_time(T2_RANGE(0), T2_RANGE(1));

        if (service_unit()) {
            su.out_requests_cnt++;
            is_request_out = true;

            su.requests_in_queue.cnt++;
            su.requests_in_queue.sum += request + simple_time;

            su.modeling_time += is_T1_lower_T2 ? modeling_time : (simple_time * (1 / (1 - CHANCE)));
            su.simple_time += is_T1_lower_T2 ? simple_time : 0;
        } else {
            aq_push(&queue, request + simple_time);
            is_request_out = false;
        }
        su.service_unit_triggers_cnt++;

        su.requests.cnt++;
        su.requests.sum += aq_len(&queue);

        if (!(su.out_requests_cnt % REQUESTS_INFO_STEP) && su.out_requests_cnt && is_request_out && !get_mes)
            print_queue_info(&su, aq_len(&queue));
    }
    ssize_t end = microseconds_now();

    if (!get_mes) print_full_service_unit_info(&su);

    if (get_mes) {
        mes->mem = sizeof(queue);
        mes->time = (end - beg) * 1000;
    }
    
    va_end(is_mes);
    return OK;
}

err_t arr_service_call(bool get_mes, ...) {
    va_list is_mes;
    err_t err = OK;
    addresses_t *adr = adr_init();

    va_start(is_mes, get_mes);

    mes_t *mes;
    if (get_mes) {
        mes = va_arg(is_mes, mes_t (*));    
        mes->mem = 0;
        mes->time = 0;
    }

    if (!get_mes) printf("ARRAY QUEUE SERVICE:\n\n");

    err = get_mes 
    ? arr_service_unit(&adr, get_mes, mes)
    : arr_service_unit(&adr, get_mes);

    va_end(is_mes);

    if (!err && !get_mes)
        FRAG_ADRESSES_PRINT(adr);

    adr_free(adr);
    return err;
}

static err_t list_service_unit(addresses_t **adrs, bool get_mes, ...) {
    RESET_RAND;
    va_list is_mes;
    err_t err = OK;
    lqueue_t *queue = lq_init();
    bool is_request_out = false;
    service_info_t su = info_init();

    va_start(is_mes, get_mes);

    mes_t *mes;
    if (get_mes) {
        mes = va_arg(is_mes, mes_t (*));    
        mes->mem = 0;
        mes->time = 0;
    }

    ssize_t beg = microseconds_now();
    if (CHANCE >= 1) {
        printf("Noone request from 1000 went out from queue\n");
        ERR_GOTO(err, ERR_MEM, error_exit);
    }

    while (su.out_requests_cnt != REQUESTS_CNT) {
        double modeling_time = get_time(T1_RANGE(0), T1_RANGE(1));
        if (add_request()) {
            if ((err = lq_push(&queue, 0)))
                goto error_exit;
            su.in_requests_cnt++;
        }

        
        double request = lq_pop(&queue);
        if (request < 0) continue;
        request += (request < EPS) ? 0 : modeling_time;
        
        if (queue)
            if (add_address(adrs, queue))
                ERR_GOTO(err, ERR_MEM, error_exit);
        
        double simple_time = get_time(T2_RANGE(0), T2_RANGE(1));

        if (service_unit()) {
            su.out_requests_cnt++;
            is_request_out = true;

            su.requests_in_queue.cnt++;
            su.requests_in_queue.sum += request + simple_time;

            su.modeling_time += (is_T1_lower_T2 ? modeling_time : simple_time) * (1 / (1 - CHANCE));
            su.simple_time += is_T1_lower_T2 ? simple_time : 0;
        } else {
            if ((err = lq_push(&queue, 0)))
                goto error_exit;
            if (get_mes) mes->mem += sizeof(*queue);
            is_request_out = false;
        }
        su.service_unit_triggers_cnt++;

        su.requests.cnt++;
        su.requests.sum += lq_len(queue);

        if (!(su.out_requests_cnt % REQUESTS_INFO_STEP) && su.out_requests_cnt && is_request_out && !get_mes)
            print_queue_info(&su, lq_len(queue));
    }
    ssize_t end = microseconds_now();

    if (!get_mes) print_full_service_unit_info(&su);

    if (get_mes) mes->time = (end - beg) * 1000;
    
    error_exit:
    lq_free(queue);
    va_end(is_mes);
    return OK;
}

err_t list_service_call(bool get_mes, ...) {
    va_list is_mes;
    err_t err = OK;
    addresses_t *adr = adr_init();

    va_start(is_mes, get_mes);

    mes_t *mes;
    if (get_mes) {
        mes = va_arg(is_mes, mes_t (*));    
        mes->mem = 0;
        mes->time = 0;
    }

    if (!get_mes) printf("LIST QUEUE SERVICE:\n\n");

    err = get_mes 
    ? list_service_unit(&adr, get_mes, mes)
    : list_service_unit(&adr, get_mes);

    va_end(is_mes);

    if (!err && !get_mes)
        FRAG_ADRESSES_PRINT(adr);

    adr_free(adr);
    return err;
}
