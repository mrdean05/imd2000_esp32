#include "im2000_uart.h"
#include "obj_list.h"

#define IMD200_BAUDRATE 256000
#define IMD200_UART_TX 17
#define IMD200_UART_RX 18
#define TX_BUFFER 1024
#define RX_BUFFER 1024
#define TAG "DATA ACQUISITION"

typedef struct
{
  char *start_delimiter;
  char *net_data_length;
  char *rep_data_length;
  char *sd2;
  char *des_address;
  char *sou_address;
  char *fun_code;
  char *pro_data_unit[2];
  char *frame_checksum;
  char *end_delimiter;

} TxFrameVariable;

typedef struct
{
  uint8_t start_delimiter;
  uint8_t des_address;
  uint8_t sou_address;
  uint8_t fun_code;
  uint8_t pro_data_unit[2];
  uint8_t frame_checksum;
  uint8_t end_delimiter;

} TxFrameFixed;

typedef union
{

  TxFrameVariable variable_frame;
  TxFrameFixed fixed_frame;
} TxFrame;


/*initialize target*/
void IMD2000_uart_init(void)
{
  uart_config_t imd200_uart = {
      .baud_rate = IMD200_BAUDRATE,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
  };

  ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &imd200_uart));
  ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, IMD200_UART_TX, IMD200_UART_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
  ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, RX_BUFFER, TX_BUFFER, 0, NULL, 0));
}

void master_send_data_IMD2000(int command)
{
  TxFrame variable_frame_tx;

  variable_frame_tx.variable_frame.start_delimiter = "68";
  variable_frame_tx.variable_frame.net_data_length = "05";
  variable_frame_tx.variable_frame.rep_data_length = "05";
  variable_frame_tx.variable_frame.sd2 = "68";
  variable_frame_tx.variable_frame.sou_address = "64";
  variable_frame_tx.variable_frame.des_address = "01";
  variable_frame_tx.variable_frame.pro_data_unit[0] = "00";
  variable_frame_tx.variable_frame.end_delimiter = "16";

  switch (command)
     {
      case 0: 

    
           variable_frame_tx.variable_frame.fun_code= "D1";
           variable_frame_tx.variable_frame.pro_data_unit[1]= "00";
           variable_frame_tx.variable_frame.frame_checksum= "36";
           break;

      case 1:
        variable_frame_tx.variable_frame.fun_code= "D1";
        variable_frame_tx.variable_frame.pro_data_unit[1]= "01";
        variable_frame_tx.variable_frame.frame_checksum= "37";
        break;
 
      case 2:
           variable_frame_tx.variable_frame.fun_code= "D2";
           variable_frame_tx.variable_frame.pro_data_unit[1]= "00";
           variable_frame_tx.variable_frame.frame_checksum= "36";
           break;
      default: 
          ESP_LOGI(TAG, "ERROR");
     }

    
     char *struct2str ( TxFrame ap);
     char *data = struct2str (variable_frame_tx);
     printf ("\n structure as a string:\n\n  '%s'\n\n", data);

    uart_write_bytes(UART_NUM_1, (const char*)data, strlen(data));
     
      if (data ) free (data );
}

char *struct2str (TxFrame ap)
{
    /* get lenght of string required to hold struct values */
    size_t len = 0;
    len = snprintf (NULL, len, "%s %s %s %s %s %s %s %s %s %s %s", ap.variable_frame.start_delimiter, ap.variable_frame.net_data_length,
                                                                                         ap.variable_frame.rep_data_length, ap.variable_frame.sd2,ap.variable_frame.sou_address,
                                                                                         ap.variable_frame.des_address, ap.variable_frame.fun_code, ap.variable_frame.pro_data_unit[0],
                                                                                         ap.variable_frame.pro_data_unit[1], ap.variable_frame.frame_checksum, ap.variable_frame.end_delimiter);

    /* allocate/validate string to hold all values (+1 to null-terminate) */
    char *apstr = calloc (1, sizeof *apstr * len + 1);
    if (!apstr) {
        fprintf (stderr, "%s() error: virtual memory allocation failed.\n", __func__);
    }

    /* write/validate struct values to apstr */
    if (snprintf (apstr, len + 1, "%s %s %s %s %s %s %s %s %s %s %s", ap.variable_frame.start_delimiter, ap.variable_frame.net_data_length,
                                                                                         ap.variable_frame.rep_data_length, ap.variable_frame.sd2,ap.variable_frame.sou_address,
                                                                                         ap.variable_frame.des_address, ap.variable_frame.fun_code, ap.variable_frame.pro_data_unit[0],
                                                                                         ap.variable_frame.pro_data_unit[1], ap.variable_frame.frame_checksum, ap.variable_frame.end_delimiter) > len + 1)
    {
        fprintf (stderr, "%s() error: snprintf returned truncated result.\n", __func__);
        return NULL;
    }

    return apstr;
}


esp_err_t master_receive_data_IMD2000(void)
{

  IMD2000_TargetList_t IMD2000_received_data;
  unsigned char received_buffer[RX_BUFFER]; 
  memset(received_buffer, 0, sizeof(RX_BUFFER)); 
  int len= uart_read_bytes (UART_NUM_1, received_buffer, RX_BUFFER, 5000/ portTICK_RATE_MS);
   printf("len = %d, data = %.*s\n",len, len, received_buffer);
   
  IMD2000_Result_t IMD2000_decode_status = IMD2000_decodeTargetFrame (received_buffer, &IMD2000_received_data);
  
  if (IMD2000_decode_status != IMD2000_API_ERR_OK){
     return ESP_FAIL;
  }
  return ESP_OK;
}