#include "main.h"

static char *TAG = "ESP32_HTTP_EVENT";
static char response_data[1024]; // 自定义缓存空间储存一次响应数据
static int recived_len = 0;      // 自定义变量储存一次响应中接收到分片数据的累计偏移

// http客户端的事件处理回调函数
static esp_err_t http_client_event_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGI(TAG, "connected to web-server");
        recived_len = 0;
        break;
    case HTTP_EVENT_ON_DATA:
        if (evt->user_data)
        {
            memcpy(evt->user_data + recived_len, evt->data, evt->data_len); // 将分片的每一片数据都复制到user_data
            recived_len += evt->data_len;//累计偏移更新
        }
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGI(TAG, "finished a request and response!");
        recived_len = 0;
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "disconnected to web-server");
        recived_len = 0;
        break;
    case HTTP_EVENT_ERROR:
        ESP_LOGE(TAG, "error");
        recived_len = 0;
        break;
    default:
        break;
    }

    return ESP_OK;
}

esp_http_client_event_handle_t http_init(void)
{
    const esp_http_client_config_t cfg = {
        .url = "http://192.168.31.18:8000",
        .event_handler = http_client_event_handler,
        .user_data = response_data,
    };
    //使用http服务器配置参数对http客户端初始化
    esp_http_client_event_handle_t httpclient = esp_http_client_init(&cfg);

    // GET方法请求
    esp_http_client_set_method(httpclient, HTTP_METHOD_GET);
    esp_http_client_set_url(httpclient, "/api/test/");//url传递发送数据x=10，y=11

    return httpclient;

    // ret = esp_http_client_perform(httpclient);//启动并阻塞等待一次响应完成
    // if (ret == ESP_OK)
    // {
	// 	//一次响应完成，打印完整响应数据，即得到X+y的值
    //     printf("GET:%s\n", response_data);
    // }
    // // POST方法请求
    // esp_http_client_set_method(httpclient, HTTP_METHOD_POST);
    // esp_http_client_set_url(httpclient, "/add/");

    // //设置请求头
    // esp_http_client_set_header(httpclient, "Content-Type", "application/json");

    // //构造json数据jsonArg请求体（即发送数据x=24，y=124）
    // cJSON *pRoot = cJSON_CreateObject();
    // cJSON_AddStringToObject(pRoot, "x", "24");
    // cJSON_AddStringToObject(pRoot, "y", "124");
    // // char *jsonArg = "{\"x\":23,\"y\":34}";
    // char *jsonArg = cJSON_Print(pRoot);
    // //printf("jsonArg:%s\n", jsonArg);
    // esp_http_client_set_post_field(httpclient, jsonArg, strlen(jsonArg));

    // ret = esp_http_client_perform(httpclient);
    // if (ret == ESP_OK)
    // {
    //     //一次响应完成，打印完整响应数据，即得到X+y的值
    //     printf("POST:%s\n", response_data);
    // }
}

void http_get(esp_http_client_event_handle_t httpclient)
{
    // 在每次读取之前清空一下response_data
    memset(response_data, '\0', sizeof(response_data));
    esp_err_t ret;
    esp_http_client_set_method(httpclient, HTTP_METHOD_GET);
    esp_http_client_set_url(httpclient, "/api/test/");//url传递发送数据x=10，y=11
    ret = esp_http_client_perform(httpclient);//启动并阻塞等待一次响应完成
    if (ret == ESP_OK)
    {
		//一次响应完成，打印完整响应数据，即得到X+y的值
        printf("GET:%s\n", response_data);
        // 写入数据及获取数据长度
        partition_data_len = strlen(response_data);
        partition_write(response_data);
        if(global_clk == 0)
        {
            // 第一次获取时间信息
            cJSON *root = cJSON_Parse(response_data);
            cJSON *time = cJSON_GetObjectItemCaseSensitive(root, "now");
            global_clk = time->valuedouble;
            xTaskCreate(timer_instance, "TIMER_TASK", 4096, NULL, 1, NULL);
            // 删除节点，释放内存
            cJSON_Delete(root);
        }

    }
}

void http_clean_up(esp_http_client_event_handle_t httpclient)
{
    esp_http_client_cleanup(httpclient);//清空http客户端描述符
}