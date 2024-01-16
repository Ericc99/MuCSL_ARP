#include "main.h"

static const char *TAG = "ESP32_NVS";

void partition_write(char data[])
{
    const esp_partition_t *partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "storage");
    
    ESP_ERROR_CHECK(esp_partition_erase_range(partition, 0, partition->size));

    // char tmp[strlen(data)+1];

    // strncpy(tmp, data, strlen(data));

    // tmp[strlen(data)] = '\0';

    ESP_ERROR_CHECK(esp_partition_write(partition, 0, data, strlen(data)));
    
    printf("Data of len: %d has been written\n", strlen(data));

}

char* partition_read()
{
    const esp_partition_t *partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "storage");

    char* read_data = malloc(partition_data_len);

    char tmp[partition_data_len];

    ESP_ERROR_CHECK(esp_partition_read(partition, 0, tmp, partition_data_len));

    strncpy(read_data, tmp, partition_data_len);

    return read_data;

}

