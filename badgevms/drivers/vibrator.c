#include "driver/gpio.h"
#include "esp_log.h"
#include "vibrator.h"

#define TAG "vibrator"

#define VIBR_PIN GPIO_NUM_3

static void
vibr_set (void *ptr, bool vibrating)
{
	gpio_set_level (VIBR_PIN, vibrating);
}

static bool
vibr_get (void *ptr)
{
	return gpio_get_level (VIBR_PIN);
}

device_t *
vibrator_create (void)
{
	vibrator_device_t	*dev;
	gpio_config_t		 pin_cfg;

	ESP_LOGI (TAG, "initializing...");

	dev = calloc (1, sizeof (*dev));
	dev->device.type = DEVICE_TYPE_VIBRATOR;
	dev->device._open = NULL;
	dev->device._close = NULL;
	dev->device._write = NULL;
	dev->device._read = NULL;
	dev->device._lseek = NULL;
	dev->_set = vibr_set;
	dev->_get = vibr_get;

	ESP_LOGD (TAG, "configuring vibrator pin...");
	pin_cfg.pin_bit_mask = VIBR_PIN;
	pin_cfg.mode = GPIO_MODE_INPUT_OUTPUT;
	pin_cfg.pull_up_en = GPIO_PULLUP_ENABLE;
	pin_cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
	pin_cfg.intr_type = GPIO_INTR_DISABLE;
	gpio_config (&pin_cfg);
	vibr_set (dev, false);

	return (device_t *)dev;
}

