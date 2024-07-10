#include "main.h"

esp_mqtt_client_handle_t mqtt_client = NULL;
int pcnt_count = 0;
int16_t test = 900;
bool pcnt_updated = false;
double motor_speed = 0;

// 初始化Motor转速控制数组
double motor_speed_list[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
// 初始化PWM参数组
const int pwm_channels[6] = LEDC_CHANNEL_LIST;
const int pwm_gpios[6] = LEDC_GPIO_LIST;

// 初始化PCNT参数组
const int pcnt_gpios[4] = PCNT_GPIO_LIST;
// const int pcnt_units[5] = PCNT_UNIT_LIST;
// const int pcnt_channels[2] = PCNT_CHANNEL_LIST;

// 初始化PCNT计数数组
int pcnt_count_list[4] = {0, 0, 0, 0};
bool pcnt_updated_list[4] = {false, false, false, false};

pcnt_unit_handle_t pcnt_unit_handle = NULL;
pcnt_unit_handle_t pcnt_unit_list[4] = {NULL, NULL, NULL, NULL};

// 主函数
void app_main(void){
    // 初始化WiFi，并等待WiFi连接
    wifi_init();
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    // 创建mqtt线程
    xTaskCreate(mqtt_init, "MQTT_TASK", 4096, NULL, 1, NULL);
    // 初始化PWM通道
    pwm_init();
    // 初始化PCNT通道
    pcnt_func_init();
    // 创建PCNT计数线程
    pcnt_monitor_init();
    // 初始化pid线程
    pid_process_init();

    while(1)
    {
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
