#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_err.h"
#include "string.h"

void IMD2000_uart_init(void);
void  master_send_data_IMD2000(int command);
esp_err_t master_receive_data_IMD2000 (void);