#include "main.h"

static char* TAG = "ESP32S3_TIMER_EVENT";

// 时钟主函数
void timer_instance()
{
    int now_sec = global_clk;
    while(1)
    {
        vTaskDelay(50 / portTICK_PERIOD_MS);
        global_clk += 0.05;
        timer_check();
        if(global_clk - now_sec >= 600)
        {
            now_sec += 600;
            http_get(http_client);
        }
        // xTaskCreate(event_monitor, "EVENT_MONITOR_TASK", 4096, NULL, 1, NULL);
    }
}

// 添加一个指令所对应的两组数据
void timer_add(double time_stamp, int speed, double duration, int id)
{
    int index = -1;
    for(int i = 0; i < 32; i++){
        if(task_list_time[i] == 0.0){
            index = i;
            break;
        }
    }
    if(index != -1){
        task_list_time[index] = time_stamp;
        task_list_speed[index] = speed;
        task_list_id[index] = id;
        task_list_time[index+1] = time_stamp + duration;
        task_list_speed[index+1] = 0;
        task_list_id[index+1] = id;
    }
}

// 移除头部的一组数据
void timer_remove()
{
    for(int i = 0; i < 31; i++){
        task_list_time[i] = task_list_time[i+1];
        task_list_speed[i] = task_list_speed[i+1];
        task_list_id[i] = task_list_id[i+1];
    }
    task_list_time[31] = 0.0;
    task_list_speed[31] = 0;
    task_list_id[31] = 0;
}

// 查看Timer是否被触发
void timer_check()
{
    if(task_list_time[0] != 0.0 && global_clk >= task_list_time[0])
    {
        motor_speed = task_list_speed[0];
        char buff[64];
        if(motor_speed != 0){
            sprintf(buff, "TASK_START_ID_%d", task_list_id[0]);
        }
        else{
            sprintf(buff, "TASK_DONE_ID_%d", task_list_id[0]);
        }
        esp_mqtt_client_publish(mqtt_client, "control", buff, strlen(buff), 2, 0);
        timer_remove();
    }
}

// 清除Timer系统中现有数据
void timer_clean()
{
    for(int i=0; i < 32; i++){
        task_list_time[i] = 0.0;
        task_list_speed[i] = 0;
        task_list_id[i] = 0;
    }
}

// 查看当前Timer中的数据
void timer_print()
{
    char buff[64];
    if(task_list_time[0] == 0.0){
        sprintf(buff, "Timer task list is empty.");
        esp_mqtt_client_publish(mqtt_client, "control", buff, strlen(buff), 2, 0);
    }
    else{
        sprintf(buff, "Timer task report @ %f...", global_clk);
        esp_mqtt_client_publish(mqtt_client, "control", buff, strlen(buff), 2, 0);
        for(int i=0; i<32; i++){
            if(task_list_time[i] == 0.0){
                break;
            }
            else{
                sprintf(buff, "Task %d: Speed %d at %f.", task_list_id[i], task_list_speed[i], task_list_time[i]);
                esp_mqtt_client_publish(mqtt_client, "control", buff, strlen(buff), 2, 0);
            }
        }
    }
}

void timer_monitor()
{
    while(1){
        // 每分钟更新表单
        if(global_clk != 0 && next_task_time_stamp != 0 && global_clk > next_task_time_stamp && next_task_done == false)
        {
            // 执行任务

            // 声明任务完成
            char buff[64];
            sprintf(buff, "TASK_DONE_ID_%d", next_task_id_stamp);
            esp_mqtt_client_publish(mqtt_client, "control", buff, strlen(buff), 2, 0);
            next_task_done = true;
            
            // 暂存任务编号
            int temp_task_id;
            temp_task_id = next_task_id_stamp;

            // 获取新的任务列表
            // vTaskDelay(1000 / portTICK_PERIOD_MS);
            // http_get(http_client);
            // vTaskDelay(1000 / portTICK_PERIOD_MS);
            // vTaskDelay(5000 / portTICK_PERIOD_MS);
            // read_data();

            // // 检查更新情况,没更新的话就等他更新
            // while(temp_task_id == next_task_id_stamp){
            //     vTaskDelay(2000 / portTICK_PERIOD_MS);
            //     http_get(http_client);
            //     read_data();
            //     // ESP_LOGI(TAG, "Waiting for update...");
            //     sprintf(buff, "Waiting for update...");
            //     esp_mqtt_client_publish(mqtt_client, "control", buff, strlen(buff), 2, 0);
            // }

            // next_task_done = false;

        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}