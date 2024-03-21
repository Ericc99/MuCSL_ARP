#include "main.h"

esp_mqtt_client_handle_t mqtt_client = NULL;
int16_t pcnt_count = 0;
bool pcnt_updated = false;
double motor_speed = 0;

// 主函数
void app_main(void){
    wifi_init();
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    xTaskCreate(mqtt_init, "MQTT_TASK", 4096, NULL, 1, NULL);
    pwm_init();
    pcnt_func_init();
    xTaskCreate(pcnt_monitor, "PCNT_TASK", 4096, NULL, 1, NULL);
    xTaskCreate(PID_init, "PID_TASK", 4096, NULL, 1, NULL);
}
