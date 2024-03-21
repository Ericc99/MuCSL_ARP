#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_system.h"
#include "esp_http_client.h"
#include "esp_partition.h"
#include "cJSON.h"
#include "driver/ledc.h"
#include "driver/pcnt.h"

#define WIFI_SSID "Get_fries_on_dock"
#define WIFI_PASS "12345677"

// PWM 参数设计
#define LEDC_TIMER      LEDC_TIMER_0
#define LEDC_MODE       LEDC_LOW_SPEED_MODE
#define LEDC_GPIO       (5)
#define LEDC_CHANNEL    LEDC_CHANNEL_0
#define LEDC_DUTY_RES   LEDC_TIMER_13_BIT
#define LEDC_DUTY       (8192)
#define LEDC_FREQ       (5000)

// PCNT 参数设置
#define PCNT_GPIO           (10)
#define PCNT_CONTROL_GPIO   (-1)
#define PCNT_CHANNEL        PCNT_CHANNEL_0
#define PCNT_UNIT           PCNT_UNIT_0
#define PCNT_POS_MODE       PCNT_COUNT_INC
#define PCNT_NEG_MODE       PCNT_COUNT_DIS

// MQTT Client 全局调用
extern esp_mqtt_client_handle_t mqtt_client;

// MQTT 传递motor参数用struct
typedef struct
{
    int start_speed;
    int duration;
    int end_speed;
} cmd_params;

// PCNT 计数参数 在PCNT和PID之间传递信息
extern int16_t pcnt_count;
// PCNT 更新参数 保证PCNT测量和PID控制同步
extern bool pcnt_updated;
// Motor 预设转速全局参数
extern double motor_speed;


// WIFI 连接方法
void wifi_init(void);

// MQTT 连接方法
void mqtt_init();
// void read_data();

// PWM 方法
void pwm_init();
void pwm_set_duty(int data);

// PCNT 方法
void pcnt_func_init();
void pcnt_monitor();

// PID 参数
struct PID_params{
    // Kp - 比例参数
    double Kp;
    // Ki - 积分参数
    double Ki;
    // Kd - 微分参数
    double Kd;
    // max, min - 控制参数上、下限(PWM)
    double max_pwm;
    double min_pwm;
    // max, min - 实际转速上下限(PCNT)
    double max_pcnt;
    double min_pcnt;
};

// PID 数据
struct PID_data{
    // integral 累计积分
    double integral;
    // 历史误差
    double pre_error;
    // 上次输入
    double pre_input;
};

// PID 方法
double PID_Calculate(struct PID_params params, struct PID_data *data, double target_speed, double current_speed);
// 初始化PID控制器
void PID_init();
// 创建一个控制任务
void control_cmd(void *params);