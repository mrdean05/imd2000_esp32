#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "im2000_uart.h"

enum send_command {
  START_ACQUISITION=    0,
  STOP_ACQUISITION =    1,
  READ_SERIAL_NUMBER =  2
};

void IMD2000_task(void *params){

  master_send_data_IMD2000(START_ACQUISITION);
  master_send_data_IMD2000(START_ACQUISITION);

  while (1){
    master_send_data_IMD2000(READ_SERIAL_NUMBER);
    esp_err_t data_decoded = master_receive_data_IMD2000();
     vTaskDelay(50/portTICK_PERIOD_MS);
  }
}

void app_main(void)
{
   
  IMD2000_uart_init();
  xTaskCreate (&IMD2000_task, "IMD2000_task", 4096, NULL, 1, NULL);

}
  