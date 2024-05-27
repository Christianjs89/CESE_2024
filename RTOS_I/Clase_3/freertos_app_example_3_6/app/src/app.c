/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file   : app.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes. */
#include "main.h"
#include "cmsis_os.h"

/* Demo includes. */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes. */
#include "board.h"
#include "led_button.h"
#include "task_function.h"

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_sys	= " RTOS - Event-Triggered Systems (ETS)\r\n";
const char *p_app	= " freertos_app_example_3_6: Demo Code\r\n";

/********************** external data declaration *****************************/
/* Declare a variable of type QueueHandle_t. This is used to reference queues*/

/* Declare a variable of type xSemaphoreHandle (binary or counting) or mutex. 
 * This is used to reference the semaphore that is used to synchronize a thread
 * with other thread or to ensure mutual exclusive access to...*/

/* Declare a variable of type TaskHandle_t. This is used to reference threads. */
TaskHandle_t task_a_h;
TaskHandle_t task_b_h;
TaskHandle_t task_c_h;

/********************** external functions definition ************************/
void app_init(void)
{
	/* Print out: Application Initialized */
	LOGGER_LOG("\r\n");
	LOGGER_LOG("%s is running - Tick [mS] = %d\r\n", GET_NAME(app_init), (int)xTaskGetTickCount());

	LOGGER_LOG(p_sys);
	LOGGER_LOG(p_app);

    /* Before a queue or semaphore (binary or counting) or mutex is used it must 
     * be explicitly created */

    /* Check the queue or semaphore (binary or counting) or mutex was created 
     * successfully. */

    /* Add queue or semaphore (binary or counting) or mutex to registry. */

	/* Add threads, ... */
	ldx_btn_config_t *p_ldx_btn_config;
    BaseType_t ret;

    p_ldx_btn_config = &ldx_btn_config[0];
    /* Task A thread at priority 1 */
    ret = xTaskCreate(task_function,					/* Pointer to the function thats implement the task. */
					  "Task A",							/* Text name for the task. This is to facilitate debugging only. */
					  (2 * configMINIMAL_STACK_SIZE),	/* Stack depth in words. */
					  (void *)p_ldx_btn_config,   		/* Pass the pointer as the task parameter. */
					  (tskIDLE_PRIORITY + 1UL),			/* This task will run at priority 1. */
					  &task_a_h);						/* We are using a variable as task handle. */

    /* Check the thread was created successfully. */
    configASSERT(ret == pdPASS);

    p_ldx_btn_config = &ldx_btn_config[1];
    /* Task B thread at priority 1 */
    ret = xTaskCreate(task_function,					/* Pointer to the function thats implement the task. */
					  "Task B",							/* Text name for the task. This is to facilitate debugging only. */
					  (2 * configMINIMAL_STACK_SIZE),	/* Stack depth in words. */
					  (void *)p_ldx_btn_config,    		/* Pass the pointer as the task parameter. */
					  (tskIDLE_PRIORITY + 1UL),			/* This task will run at priority 1. */
					  &task_b_h);						/* We are using a variable as task handle. */

    /* Check the thread was created successfully. */
    configASSERT(ret == pdPASS);

    p_ldx_btn_config = &ldx_btn_config[2];
    /* Task C thread at priority 1 */
    ret = xTaskCreate(task_function,					/* Pointer to the function thats implement the task. */
					  "Task C",							/* Text name for the task. This is to facilitate debugging only. */
					  (2 * configMINIMAL_STACK_SIZE),	/* Stack depth in words. */
					  (void *)p_ldx_btn_config,    		/* Pass the pointer as the task parameter. */
					  (tskIDLE_PRIORITY + 1UL),			/* This task will run at priority 1. */
					  &task_c_h);						/* We are using a variable as task handle. */

    /* Check the thread was created successfully. */
    configASSERT(ret == pdPASS);

    cycle_counter_init();
}

/********************** end of file ******************************************/
