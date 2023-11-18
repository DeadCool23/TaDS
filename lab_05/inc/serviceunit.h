#ifndef __SERVICEUNIT_H__
#define __SERVICEUNIT_H__

#include "errs.h"
#include "addressfrag.h"

#include <stddef.h>
#include <stdbool.h>

#define REQUESTS_CNT 1000        // Число обработанных заявок
#define REQUESTS_INFO_STEP 100   // Число заявок, после которого нужно предоставить информацию о состоянии ОА

typedef struct {
    size_t cnt;                              // Количество длин очередей
    size_t sum;                              // Сумма длин очередей
} queue_lens_inf_t;

typedef struct {
    size_t cnt;                              // Количество заявок вышедших из очереди
    double sum;                              // Сумма времени заявок в очереди
} time_in_queue_inf_t;

/// @brief Структура Обрабатывающего аппарата
typedef struct {
    double simple_time;                      // Время простоя ОА
    queue_lens_inf_t requests;               // Информация об очереди
    time_in_queue_inf_t requests_in_queue;   // Информация об времени заявки в очереди
    size_t in_requests_cnt;                  // Количество вошедших заявок
    size_t out_requests_cnt;                 // Количество вышедших заявок
    size_t service_unit_triggers_cnt;        // Количество срабатываний ОА
    double modeling_time;                    // Время моделирования
} service_info_t;

err_t arr_service_call(bool get_mes, ...);
err_t list_service_call(bool get_mes, ...);


#endif // __SERVICEUNIT_H__
