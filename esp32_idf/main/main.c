#include "main.h"

int partition_data_len = 0;
double global_time_stamp = 0.0;
double next_task_time_stamp = 0.0;
int next_task_id_stamp = 0;
esp_http_client_event_handle_t http_client = NULL;
esp_mqtt_client_handle_t mqtt_client = NULL;
bool temp_bool = false;
int16_t pcnt_count = 0;
bool pcnt_updated = false;
double motor_speed = 0;
bool PID_bool = false;

void app_main(void){
    wifi_init();
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    http_client = http_init();
    xTaskCreate(mqtt_init, "MQTT_TASK", 4096, NULL, 1, NULL);
    pwm_init();
    pcnt_func_init();
    xTaskCreate(pcnt_monitor, "PCNT_TASK", 4096, NULL, 1, NULL);
}
