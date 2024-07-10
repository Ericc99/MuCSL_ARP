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
#include "driver/pulse_cnt.h"

#define WIFI_SSID "Get_fries_on_dock"
#define WIFI_PASS "12345677"

// PWM 参数定义
// 计时器参数
#define LEDC_TIMER      LEDC_TIMER_0
#define LEDC_MODE       LEDC_LOW_SPEED_MODE
#define LEDC_DUTY_RES   LEDC_TIMER_13_BIT
#define LEDC_DUTY       (8192)
#define LEDC_FREQ       (5000)

// PWM通道参数
#define LEDC_GPIO_LIST  {5, 6, 7, 8, 9, 10}
#define LEDC_CHANNEL_LIST {LEDC_CHANNEL_0, LEDC_CHANNEL_1, LEDC_CHANNEL_2, LEDC_CHANNEL_3, LEDC_CHANNEL_4, LEDC_CHANNEL_5}

extern const int pwm_channels[6];
extern const int pwm_gpios[6];


// PCNT 参数设置
// 基础参数
#define PCNT_CONTROL_GPIO   (-1)
#define PCNT_POS_MODE       PCNT_COUNT_INC
#define PCNT_NEG_MODE       PCNT_COUNT_DIS

// 通道参数
#define PCNT_GPIO_LIST      {11, 12, 13, 14}
// #define PCNT_UNIT_LIST      {PCNT_UNIT_0, PCNT_UNIT_1, 2, PCNT_UNIT_3, 4}
// #define PCNT_CHANNEL_LIST  {PCNT_CHANNEL_0, PCNT_CHANNEL_1}

extern const int pcnt_gpios[4];
extern const int pcnt_units[5];
extern const int pcnt_channels[2];

// PCNT 计数器相关变量
extern int pcnt_count_list[4];
extern bool pcnt_updated_list[4];



#define PCNT_GPIO           (11)
#define PCNT_CHANNEL        PCNT_CHANNEL_0
#define PCNT_UNIT           PCNT_UNIT_0

// PCNT_2 参数设置
#define PCNT_GPIO_2           (11)
// #define PCNT_CONTROL_GPIO   (-1)
#define PCNT_CHANNEL_2        PCNT_CHANNEL_1
#define PCNT_UNIT_2           PCNT_UNIT_0
// #define PCNT_POS_MODE       PCNT_COUNT_INC
// #define PCNT_NEG_MODE       PCNT_COUNT_DIS

extern pcnt_unit_handle_t pcnt_unit_handle;

extern pcnt_unit_handle_t pcnt_unit_list[4];


// MQTT Client 全局调用
extern esp_mqtt_client_handle_t mqtt_client;

// MQTT 传递motor参数用struct
typedef struct
{
    int speed;
    int duration;
} cmd_params;

// PCNT 参数组
typedef struct {
    
} pcnt_params;


// PCNT 计数参数 在PCNT和PID之间传递信息
extern int pcnt_count;
// PCNT 更新参数 保证PCNT测量和PID控制同步
extern bool pcnt_updated;
// Motor 预设转速全局参数
extern double motor_speed;

// Motor 转速控制全局数组
extern double motor_speed_list[6];


// WIFI 连接方法
void wifi_init(void);

// MQTT 连接方法
void mqtt_init();

// PWM 方法
void pwm_init();
void pwm_set_duty(int data, int channel);

// PCNT 方法
void pcnt_func_init();
void pcnt_monitor_init();
void pcnt_monitor(void* params);

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
void PID_init(void* params);
// PID线程初始化
void pid_process_init();
// 创建一个控制任务
void control_cmd(void *params);