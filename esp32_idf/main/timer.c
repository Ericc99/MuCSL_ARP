#include "main.h"

static char* TAG = "ESP32S3_TIMER_EVENT";

void timer_instance()
{
    int now_sec = global_time_stamp;
    while(1)
    {
        vTaskDelay(50 / portTICK_PERIOD_MS);
        global_time_stamp += 0.05;
        if(global_time_stamp - now_sec >= 60)
        {
            now_sec += 10;
            http_get(http_client);
        }
        // xTaskCreate(event_monitor, "EVENT_MONITOR_TASK", 4096, NULL, 1, NULL);
    }
}

void event_monitor()
{
    int now_sec = global_time_stamp;
    while(1){
        // 每分钟更新表单
        if(global_time_stamp != 0 && next_task_time_stamp != 0 && global_time_stamp > next_task_time_stamp && next_task_done == false)
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