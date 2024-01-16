#include "main.h"

// 这里的PID控制针对于以下过程
// -- 转速 --> PID 控制器 --> PWM 控制输入 --> PCNT 转速测量 -->
//          ^                                     |
//          |                                     |
//          ---------------------------------------

void PID_Calculate(struct PID_params params, struct PID_data *data, double set_speed, double current_speed)
{
    // 计算e(t)
    double error = set_speed - current_speed;
    // 比例项
    double Pout = params.Kp * error;
    // 积分项
    data -> integral += error * params.dt;
    double Iout = params.Ki * data -> integral;
    // 微分项
    double derivative = (error - data -> pre_error) / params.dt;
    double Dout = params.Kd * derivative;

    // 计算整体输出
    double output = Pout + Iout + Dout;

    // 限制条件
    if(output > params.max)
    {
        output = params.max;
    }
    else if(output < params.min)
    {
        output = params.min;
    }

    // 保存本次误差到上次
    data -> pre_error = error;

    return output;
}


