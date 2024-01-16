#include "main.h"

void timer_instance()
{
    int now_sec = global_time_stamp;
    while(1)
    {
        vTaskDelay(50 / portTICK_PERIOD_MS);
        global_time_stamp += 0.05;
        if(global_time_stamp - now_sec > 60)
        {
            // 每一分钟更新一次表单
            now_sec += 60;
            http_get(http_client);
            // printf("Current time: %d\n", now_sec);
        }
        if(global_time_stamp != 0 && next_task_time_stamp != 0 && global_time_stamp > next_task_time_stamp && temp_bool == false)
        {
            // Do the operation

            // Send the message declaring operation is done
            char buff[64];
            sprintf(buff, "Task %d has been done on time %f.", next_task_id_stamp, next_task_time_stamp);
            printf("%s\n", buff);
            esp_mqtt_client_publish(mqtt_client, "control", buff, strlen(buff), 2, 0);
            temp_bool = true;
        }
    }
}

void event_monitor()
{

}