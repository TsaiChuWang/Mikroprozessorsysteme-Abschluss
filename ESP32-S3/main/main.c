
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "sdkconfig.h"

#define TRUE 1
#define IMAGE_SIZE 28
#define BUFFER_SIZE 64
#define NOT_FOUND -1
/* Constants that aren't configurable in menuconfig */
#define WEB_SERVER "192.168.50.120"
#define WEB_PORT "8000"
#define WEB_PATH "/isClassificate"

int returnCharIndex(char* buffer, int buffer_size, char specified_char);

static const char *TAG = "ESP32-S3";

static const char *REQUEST = "GET " WEB_PATH " HTTP/1.0\r\n"
    "Host: "WEB_SERVER":"WEB_PORT"\r\n"
    "User-Agent: esp-idf/1.0 esp32\r\n"
    "\r\n";

static void http_get_task(void *pvParameters)
{
    const struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *response;
    struct in_addr *address;
    int _socket, recieve;
    char recieve_buffer[BUFFER_SIZE];

    while(TRUE) {
        int error = getaddrinfo(WEB_SERVER, WEB_PORT, &hints, &response);
        if(error != 0 || response == NULL) {
            ESP_LOGE(TAG, "DNS lookup failed error=%d response=%p", error, response);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        address = &((struct sockaddr_in *)response->ai_addr)->sin_addr;

        _socket = socket(response->ai_family, response->ai_socktype, 0);
        if(_socket < 0) {
            ESP_LOGE(TAG, "... Failed to allocate socket.");
            freeaddrinfo(response);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        if(connect(_socket, response->ai_addr, response->ai_addrlen) != 0) {
            ESP_LOGE(TAG, "... socket connect failed errno=%d", errno);
            close(_socket);
            freeaddrinfo(response);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        freeaddrinfo(response);

        if (write(_socket, REQUEST, strlen(REQUEST)) < 0) {
            ESP_LOGE(TAG, "... socket send failed");
            close(_socket);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }

        struct timeval receiving_timeout;
        receiving_timeout.tv_sec = 5;
        receiving_timeout.tv_usec = 0;
        if (setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout,
                sizeof(receiving_timeout)) < 0) {
            ESP_LOGE(TAG, "... failed to set socket receiving timeout");
            close(_socket);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        int cou = 0;
        /* Read HTTP response */
        do {
            bzero(recieve_buffer, sizeof(recieve_buffer));
            recieve = read(_socket, recieve_buffer, sizeof(recieve_buffer)-1);
            // printf("cou=%d\n\n", cou);
            for(int i = 0; i < recieve; i++) {
                putchar(recieve_buffer[i]);
            }
            // printf("recive=%d\n",recieve);
            // ESP_LOGI(TAG, "index{ = %d", returnCharIndex(recieve_buffer,recieve,'{'));
            cou+=1;
        } while(recieve > 0);
        // printf("cou=%d\n", cou);
        int response_code = (int)recieve_buffer[0];
        // ESP_LOGI(TAG, "recive code = %d,%c", response_code, recieve_buffer[0]);
        if(response_code!=0){
            // for(int index=0;index<IMAGE_SIZE;index++)
            for(int i = 0; i < recieve; i++) {
                putchar(recieve_buffer[i]);
            }
            printf("\n");
            ESP_LOGI(TAG, "index{ = %d", returnCharIndex(recieve_buffer,recieve,'{'));
        }
        // ESP_LOGI(TAG, "index{ = %d", returnCharIndex(recieve_buffer,recieve,'{'));
        // ESP_LOGI(TAG, "buffer=%s\n", recieve_buffer);
        close(_socket);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // ESP_LOGI(TAG, "Starting again!");
    }
}


void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init() );
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(example_connect());

    xTaskCreate(&http_get_task, "http_get_task", 9012, NULL, 5, NULL);
}

int returnCharIndex(char* buffer, int buffer_size, char specified_char){
    for(int index=0;index<buffer_size;index++)
        if(*(buffer+index)==specified_char)
            return index;

    return NOT_FOUND;
}