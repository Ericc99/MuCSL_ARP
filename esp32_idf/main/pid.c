#include "main.h"

// 这里的PID控制针对于以下过程
// -- 转速 --> PID 控制器 --> PWM 控制输入 --> PCNT 转速测量 -->
//          ^                                     |
//          |                                     |
//          ---------------------------------------

double PID_Calculate(struct PID_params params, struct PID_data *data, double target_speed, double current_speed)
{
    // 计算Error
    double error = target_speed - current_speed;

    // 比例项
    double Pout = params.Kp * error;
    
    // 积分项
    data -> integral += error;
    // 限制积分项，防止积分饱和
    if(data -> integral > params.max_pwm)
    {
        data -> integral = params.max_pwm;
    }
    if(data -> integral < params.min_pwm)
    {
        data -> integral = params.min_pwm;
    }
    double Iout = params.Ki * data -> integral;

    // 微分项
    double derivative = (error - data -> pre_error);
    double Dout = params.Kd * derivative;

    // 计算整体输出
    double output = Pout + Iout + Dout;
    output = data -> pre_input + output;

    // 限制条件
    if(output > params.max_pwm)
    {
        output = params.max_pwm;
    }
    else if(output < params.min_pwm)
    {
        output = params.min_pwm;
    }

    // 保存本次误差到上次
    data -> pre_error = error;
    data -> pre_input = output;

    return output;
}

// 初始化PID控制器
void PID_init()
{
    struct PID_data data = {
        .integral   = 0,
        .pre_error  = 0,
        .pre_input = 0
    };

    struct PID_params params = {
        .Kp         = 8,
        .Ki         = 0.02,
        .Kd         = 0.01,
        .max_pwm    = 8192,
        .min_pwm    = 0,
        .max_pcnt   = 435,
        .min_pcnt   = -435
    };

    while(1){
        if(pcnt_updated == true)
        {
            double temp = motor_speed;
            double new_input = PID_Calculate(params, &data, temp, pcnt_count);
            int new_input_int = 8192 - (int)new_input;
            pwm_set_duty(new_input_int);
            pcnt_updated = false;
        }
        else{
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
}

// 创建一个控制任务
void control_cmd(void *params)
{
    cmd_params* local_params = (cmd_params*)params;
    char buff[64];
    sprintf(buff, "Initial Speed: %d\nDuration: %d\nEnding Speed: %d\n", local_params->start_speed, local_params->duration, local_params->end_speed);
    esp_mqtt_client_publish(mqtt_client, "cmd", buff, strlen(buff), 2, 0);
    motor_speed = local_params -> start_speed;
    vTaskDelay(local_params -> duration * 1000 / portTICK_PERIOD_MS);
    motor_speed = local_params -> end_speed;
    sprintf(buff, "Task Finished.");
    esp_mqtt_client_publish(mqtt_client, "task_manager", buff, strlen(buff), 2, 0);
    vTaskDelete(NULL);
}



