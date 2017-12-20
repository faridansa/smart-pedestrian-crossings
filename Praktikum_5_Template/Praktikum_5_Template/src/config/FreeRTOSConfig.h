/**
 * Sample Config (for ATXMega256A3BU)
 * See http://www.freertos.org/a00110.html for mor information
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <avr/io.h>

#define configUSE_PREEMPTION                            1
#define configCPU_CLOCK_HZ                              ( ( unsigned long ) 32000000 )
#define configTICK_RATE_HZ                              ( ( TickType_t ) 500 )
#define configMAX_PRIORITIES                            5
#define configMINIMAL_STACK_SIZE                        ( ( unsigned short ) 85 )
#define configMAX_TASK_NAME_LEN                         ( 8 )
#define configUSE_16_BIT_TICKS                          1
#define configIDLE_SHOULD_YIELD                         1

/* Hook function related definitions. */
#define configUSE_IDLE_HOOK                             0
#define configUSE_TICK_HOOK                             0

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                           0
#define configMAX_CO_ROUTINE_PRIORITIES                 5

/* Optional functions - most linkers will remove unused functions anyway. */
#define INCLUDE_vTaskPrioritySet                        0
#define INCLUDE_uxTaskPriorityGet                       0
#define INCLUDE_vTaskDelete                             0
#define INCLUDE_vTaskSuspend                            0
#define INCLUDE_vTaskDelayUntil                         1
#define INCLUDE_vTaskDelay                              1

/* Software timer related definitions. */
#define configUSE_TIMERS 1
#define configTIMER_TASK_PRIORITY 3
#define configTIMER_QUEUE_LENGTH 10
#define configTIMER_TASK_STACK_DEPTH configMINIMAL_STACK_SIZE

#endif /* FREERTOS_CONFIG_H */
