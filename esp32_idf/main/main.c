#include "main.h"

int partition_data_len = 0;
double global_clk = 0.0;
double next_task_time_stamp = 0.0;
int next_task_id_stamp = 0;
double next_task_duration = 0;
esp_http_client_event_handle_t http_client = NULL;
esp_mqtt_client_handle_t mqtt_client = NULL;
bool next_task_done = false;
int16_t pcnt_count = 0;
bool pcnt_updated = false;
double motor_speed = 0;
bool PID_bool = false;
double task_list_time[32] = {0.0};
int task_list_speed[32] = {0};
int task_list_id[32] = {0};

void app_main(void){
    wifi_init();
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    http_client = http_init();
    xTaskCreate(mqtt_init, "MQTT_TASK", 4096, NULL, 1, NULL);
    pwm_init();
    pcnt_func_init();
    xTaskCreate(pcnt_monitor, "PCNT_TASK", 4096, NULL, 1, NULL);
    xTaskCreate(PID_controller, "PID_CONTROLLER_TASK", 4096, NULL, 1, NULL);
}
