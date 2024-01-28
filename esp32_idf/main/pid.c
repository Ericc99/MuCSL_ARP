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
void PID_controller()
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
            if(motor_speed == 0)
            {
                data.integral = 0;
                data.pre_error = 0;
                data.pre_input = 0;
                pwm_set_duty(8192);
                pcnt_updated = false;
            }
            double temp = motor_speed;
            double new_input = PID_Calculate(params, &data, temp, pcnt_count);
            // char buff[64];
            // sprintf(buff, "Calculated input is: %f", new_input);
            // esp_mqtt_client_publish(mqtt_client, "control", buff, strlen(buff), 2, 0);
            int new_input_int = 8192 - (int)new_input;
            pwm_set_duty(new_input_int);
            pcnt_updated = false;
        }
        else{
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
}


