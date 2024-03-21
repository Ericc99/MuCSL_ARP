#include "main.h"

static char* TAG = "ESP32S3_MQTT_EVENT";

static bool connect_flag = false; // 定义一个连接上mqtt服务器的flag

void message_compare(char *msg)
{
    if(strncmp(msg, "pwm_", 4) == 0)
    {
        char tmp[5];
        strcpy(tmp, msg + 4);
        int new_duty = atoi(tmp);
        motor_speed = new_duty;
    }
    else if(strncmp(msg, "cmd_", 4) == 0)
    {
        int speed_1, duration, speed_2;
        sscanf(msg, "cmd_%d_%d_%d", &speed_1, &duration, &speed_2);
        cmd_params params = {speed_1, duration, speed_2};
        xTaskCreate(control_cmd, "CMD_TASK", 4096, (void*)&params, 1, NULL);

    }
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

void mqtt_init()
{
    // mqtt服务器的配置信息
    esp_mqtt_client_config_t cfg = {
        .broker.address = {
            // EMQX 服务器主机IP地址
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