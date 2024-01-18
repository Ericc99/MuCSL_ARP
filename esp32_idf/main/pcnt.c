#include "main.h"

static const char* TAG = "PCNT_EVENT";

// PCNT 初始化
// 注意貌似pcnt_init()这个函数名已经被内部函数占用了，如果命名为pcnt_init()会奇妙的报错
void pcnt_func_init()
{
    pcnt_config_t pcnt_config = {
        .pulse_gpio_num     = PCNT_GPIO,
        .ctrl_gpio_num      = PCNT_CONTROL_GPIO,
        .channel            = PCNT_CHANNEL,
        .unit               = PCNT_UNIT,
        .pos_mode           = PCNT_POS_MODE,
        .neg_mode           = PCNT_NEG_MODE
    };
    pcnt_unit_config(&pcnt_config);
}

void pcnt_monitor()
{
    bool idle = false;
    while(1)
    {
        // 每一秒进行一次计数，并且清除之前的计数
        pcnt_get_counter_value(PCNT_UNIT, &pcnt_count);
        pcnt_counter_clear(PCNT_UNIT);
        ESP_LOGI(TAG, "PCNT count: %d", pcnt_count);
        if(motor_speed == 0 && idle == false)
        {
            char buff[64];
            sprintf(buff, "PCNT count: %d", pcnt_count);
            esp_mqtt_client_publish(mqtt_client, "control", buff, strlen(buff), 2, 0);
            idle = true;
        }
        else if(motor_speed != 0)
        {
            char buff[64];
            sprintf(buff, "PCNT count: %d", pcnt_count);
            esp_mqtt_client_publish(mqtt_client, "control", buff, strlen(buff), 2, 0);
            idle = false;
        }
        pcnt_updated = true;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}