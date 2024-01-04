
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// #include "driver/sdmmc_host.h"
// #include "driver/sdmmc_defs.h"
// #include "sdmmc_cmd.h"
// #include "esp_vfs_fat.h"


#include "../include/configuration.h"
#include "../include/histogram.h"
#include "esp_camera.h"

static const char *TAG = "SD_Karte_SPEICHERN";

static camera_config_t camera_config = {
    .pin_pwdn = CONFIG_PWDN,
    .pin_reset = CONFIG_RESET,
    .pin_xclk = CONFIG_XCLK,
    .pin_sscb_sda = CONFIG_SDA,
    .pin_sscb_scl = CONFIG_SCL,

    .pin_d7 = CONFIG_D7,
    .pin_d6 = CONFIG_D6,
    .pin_d5 = CONFIG_D5,
    .pin_d4 = CONFIG_D4,
    .pin_d3 = CONFIG_D3,
    .pin_d2 = CONFIG_D2,
    .pin_d1 = CONFIG_D1,
    .pin_d0 = CONFIG_D0,
    .pin_vsync = CONFIG_VSYNC,
    .pin_href = CONFIG_HREF,
    .pin_pclk = CONFIG_PCLK,

    //XCLK 20MHz or 10MHz
    .xclk_freq_hz = CONFIG_XCLK_FREQ,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_GRAYSCALE, //YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size = FRAMESIZE_96X96,   //QQVGA-UXGA Do not use sizes above QVGA when not JPEG

    .jpeg_quality = 12, //0-63 lower number means higher quality
    .fb_count = 1       //if more than one, i2s runs in continuous mode. Use only with JPEG
};

static esp_err_t init_camera()
{
  esp_err_t err = esp_camera_init(&camera_config);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Camera Init Failed");
    return err;
  }
  return ESP_OK;
}

void app_main()
{
  init_camera();
  ESP_LOGI(TAG, "INit camera success, Taking picture...\n");

  camera_fb_t *pic = esp_camera_fb_get();

  u_int8_t* bilder = (u_int8_t*)malloc(sizeof(u_int8_t)*BESONDERHEIT_ZAHL);
  for(int i=0;i<BILDGRÖSSE;i++){
    for(int j=0;j<BILDGRÖSSE;j++){
      if(*(pic->buf+(i*BILDGRÖSSE+j)) > SCHWELLE)
        *(bilder+(i*BILDGRÖSSE+j)) = 1;
      else *(bilder+(i*BILDGRÖSSE+j)) = 0;
      #ifdef BINÄR_ANZEIGEN
        // printf(" %3d ", *(pic->buf+(i*BILDGRÖSSE+j)));
        printf("%d", *(bilder+(i*BILDGRÖSSE+j)));
      #endif
    }
    #ifdef BINÄR_ANZEIGEN
    printf("\n");
    #endif
  }

  float* hogMerkmale = berechnenHOG(bilder);
  free(bilder); bilder = NULL;
  anzeigenBesonderheit(hogMerkmale);
}
