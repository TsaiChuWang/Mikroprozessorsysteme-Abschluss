#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <esp_log.h>

#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"

#define SD_CMD 38
#define SD_CLK 39
#define SD_DATA 40

#define MOUNT_POINT "/sdcard"
static const char *TAG = "CNN_V0";

#include "../include/model.h"
#include "../include/network.h"
#include "../include/memtool.h"
#include "../include/typ.h"

static void init_sdcard()
{
  esp_err_t ret = ESP_FAIL;
  sdmmc_host_t host = SDMMC_HOST_DEFAULT();
  sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
  slot_config.width = 1;

  slot_config.clk = SD_CLK;
  slot_config.cmd = SD_CMD;
  slot_config.d0 = SD_DATA;

  esp_vfs_fat_sdmmc_mount_config_t mount_config = {
      .format_if_mount_failed = false,
      .max_files = 3,
  };
  sdmmc_card_t *card;

  ESP_LOGI(TAG, "Mounting SD card...");
  ret = esp_vfs_fat_sdmmc_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &card);

  if (ret == ESP_OK)
    ESP_LOGI(TAG, "\n\n\nSD card mount successfully!\n\n\n\n");
  else
    ESP_LOGE(TAG, "Failed to mount SD card VFAT filesystem. Error: %s", esp_err_to_name(ret));
}

void app_main(void)
{
    init_sdcard();

    Network * CNN = loadNetwork("/sdcard/model.sav");
    
}
