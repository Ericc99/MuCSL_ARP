#include "main.h"

static const char* TAG = "PWM_EVENT";

// 初始化PWM生成器
void pwm_init()
{
    ledc_timer_config_t ledc_timer = {
        .speed_mode         = LEDC_MODE,
        .timer_num          = LEDC_TIMER,
        .duty_resolution    = LEDC_DUTY_RES,
        .freq_hz            = LEDC_FREQ,
        .clk_cfg            = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .channel    = LEDC_CHANNEL,
        .timer_sel  = LEDC_TIMER,
        .intr_type  = LEDC_INTR_DISABLE,
        .gpio_num   = LEDC_GPIO,
        .duty       = 8192,
        .hpoint     = 0
    };
    ledc_channel_config(&ledc_channel);
}

// 改变PWM参数方法
void pwm_set_duty(int data)
{
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, data);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
    ESP_LOGI(TAG, "Duty has been set to %d.", data);
    // char buff[64];
    // sprintf(buff, "Duty has been set to %d.", data);
    // esp_mqtt_client_publish(mqtt_client, "control", buff, strlen(buff), 2, 0);
}