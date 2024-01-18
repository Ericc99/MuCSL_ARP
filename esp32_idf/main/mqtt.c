#include "main.h"

static char* TAG = "ESP32S3_MQTT_EVENT";

static bool connect_flag = false; // 定义一个连接上mqtt服务器的flag

void message_compare(char *msg)
{
    if(!strcmp("get_http", msg))
    {
        http_get(http_client);
    }
    else if(!strcmp("read_data", msg))
    {
        // // 获取NVS当中存储的JSON格式内容
        // char* read_data = partition_read();
        // // printf("Data len: %d\n", partition_data_len);
        // // printf("Read Data: %s\n", read_data);
        // // printf("Data Len: %d\n", strlen(read_data));

        // // 创建临时变量存起来，并且去掉末尾的特殊符号，添加结尾符号
        // char tmp[partition_data_len+1];
        // strncpy(tmp, read_data, partition_data_len);
        // tmp[partition_data_len] = '\0';
        // printf("Result: %s\n", tmp);

        // // 释放内存
        // free(read_data);

        // // JSON格式读取
        // cJSON *root = cJSON_Parse(tmp);
        // cJSON *data = cJSON_GetObjectItemCaseSensitive(root, "data");
        // // 读取存储大小
        // int task_size = cJSON_GetArraySize(data);
        // printf("Total %d tasks in stock.\n", task_size);
        // // 读取第一个数据时间并且存储
        // cJSON* next_task = cJSON_GetArrayItem(data, 0);
        // cJSON* next_task_id = cJSON_GetObjectItem(next_task, "id");
        // cJSON* next_task_time = cJSON_GetObjectItem(next_task, "time");
        // cJSON* next_task_dur = cJSON_GetObjectItem(next_task, "duration");
        // next_task_time_stamp = next_task_time->valuedouble;
        // next_task_id_stamp = next_task_id->valueint;
        // next_task_duration = next_task_dur->valueint;
        // // 删除节点，释放内存
        // cJSON_Delete(root);
        read_data();
    }
    else if(strncmp(msg, "pwm_", 4) == 0)
    {
        char tmp[5];
        strcpy(tmp, msg + 4);
        int new_duty = atoi(tmp);
        motor_speed = new_duty;
        // pwm_set_duty(new_duty);
        if(PID_bool == false)
        {
            xTaskCreate(PID_controller, "PID_CONTROLLER_TASK", 4096, NULL, 1, NULL);
            PID_bool = true;
        }
    }
}

void read_data()
{
    // 获取NVS当中存储的JSON格式内容
    char* read_data = partition_read();

    // 创建临时变量存起来，并且去掉末尾的特殊符号，添加结尾符号
    char tmp[partition_data_len+1];
    strncpy(tmp, read_data, partition_data_len);
    tmp[partition_data_len] = '\0';
    printf("Result: %s\n", tmp);

    // 释放内存
    free(read_data);

    // JSON格式读取
    cJSON *root = cJSON_Parse(tmp);
    cJSON *data = cJSON_GetObjectItemCaseSensitive(root, "data");
    
    // 读取存储大小
    int task_size = cJSON_GetArraySize(data);
    printf("Total %d tasks in stock.\n", task_size);
    
    // 读取第一个数据时间并且存储
    cJSON* next_task = cJSON_GetArrayItem(data, 0);
    cJSON* next_task_id = cJSON_GetObjectItem(next_task, "id");
    cJSON* next_task_time = cJSON_GetObjectItem(next_task, "time");
    cJSON* next_task_dur = cJSON_GetObjectItem(next_task, "duration");
    next_task_time_stamp = next_task_time->valuedouble;
    next_task_id_stamp = next_task_id->valueint;
    next_task_duration = next_task_dur->valueint;
    
    // 删除节点，释放内存
    cJSON_Delete(root);
}

// mqtt的状态机事件处理函数
static void mqtt_event_handler(void *args, esp_event_base_t base, int32_t id, void *event_data)
{
    esp_mqtt_event_t *client_event = event_data;
    esp_mqtt_event_id_t client_id = id;
    switch (client_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "connected to mqtt_server");
        connect_flag = true;
        esp_mqtt_client_subscribe(mqtt_client, "control", 2); // 订阅一个/control的topic
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "disconnected");
        connect_flag = false;
        break;
    case MQTT_EVENT_DATA:
    {
        char topic[128] = {};
        char data[512] = {};
        memcpy(topic, client_event->topic, client_event->topic_len);
        memcpy(data, client_event->data, client_event->data_len);
        // printf("topic:%s\n", topic);
        // printf("data:%s\n", data);
        message_compare(data);
    }
    break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "error");
        connect_flag = false;
        break;

    default:
        break;
    }
}

void mqtt_init(void *pvParameters)
{
    // mqtt服务器的配置信息（根据实际情况填写）
    esp_mqtt_client_config_t cfg = {
        .broker.address = {
            .uri = "mqtt://192.168.31.18",
            .port = 1883,
            
        },
        .credentials = {
            .username = "ESP32_1",
            
        },
        .credentials.authentication = {
            .password = "123456",
        }
    };

    mqtt_client = esp_mqtt_client_init(&cfg);

    //注册MQTT状态机事件处理回调函数
    esp_mqtt_client_register_event(mqtt_client, MQTT_EVENT_ANY, mqtt_event_handler, NULL);

    esp_mqtt_client_start(mqtt_client);

    while (1)
    {
        if (connect_flag == true)
        {
            char buff[64] = "ESP32_1 is online";
            // 向mqtt服务器发布主题为/heartbeat/1，payload为buff的数据
            esp_mqtt_client_publish(mqtt_client, "test", buff, strlen(buff), 2, 0); 
        }

        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}