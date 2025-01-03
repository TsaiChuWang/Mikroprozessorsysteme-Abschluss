
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/sdmmc_host.h"
#include "driver/sdmmc_defs.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"

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
  //initialize the camera
  esp_err_t err = esp_camera_init(&camera_config);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Camera Init Failed");
    return err;
  }

  return ESP_OK;
}

static void init_sdcard()
{
  esp_err_t ret = ESP_FAIL;
  sdmmc_host_t host = SDMMC_HOST_DEFAULT();
  sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
  esp_vfs_fat_sdmmc_mount_config_t mount_config = {
      .format_if_mount_failed = false,
      .max_files = 3,
  };
  sdmmc_card_t *card;

  ESP_LOGI(TAG, "Mounting SD card...");
  ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

  // if (ret == ESP_OK)
  // {
  //   // ESP_LOGI(TAG, "SD card mount successfully!");
  // }
  // else
  // {
  //   // ESP_LOGE(TAG, "Failed to mount SD card VFAT filesystem. Error: %s", esp_err_to_name(ret));
  // }
}

void app_main()
{
  init_sdcard();
  ESP_LOGI(TAG, "INit sdcard success\n");
  init_camera();
  ESP_LOGI(TAG, "INit camera success, Taking picture...\n");

  camera_fb_t *pic = esp_camera_fb_get();
  char *pic_name = malloc(17 + sizeof(int64_t));

  for(int i=0;i<96;i++){
    for(int j=0;j<96;j++)
      printf(" %3d ", *(pic->buf+(i*96+j)));
    printf("\n");
  }
  // sprintf(pic_name, "/sdcard/test.bmp");
  // FILE *file = fopen(pic_name, "w");

  sprintf(pic_name, "/sdcard/b.gry");
  FILE *file = fopen(pic_name, "w");

  if (file != NULL)
  {
    fwrite(pic->buf, 1, pic->len, file);
    // ESP_LOGI(TAG, "File saved: %s", pic_name);
  }else
    ESP_LOGE(TAG, "Could not open file =(");

  fclose(file);
  free(pic_name);

}
