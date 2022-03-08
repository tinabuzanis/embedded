/* LEDC (LED Controller) basic example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "driver/ledc.h"
#include <math.h>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_DUTY_RES           LEDC_TIMER_8_BIT // Set duty resolution to 13 bits
#define LEDC_FREQUENCY          (5000) // Frequency in Hertz. Set frequency at 5 kHz

unsigned long millis() {
    return (unsigned long) (esp_timer_get_time() / 1000ULL);
}

void set_duty(float intensity, int channel)
{
	int duty = (1.f -intensity) * 256;
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, channel, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, channel));
}

void set_color(float r, float g, float b)
{
	set_duty(r, LEDC_CHANNEL_0);
	set_duty(g, LEDC_CHANNEL_1);
	set_duty(b, LEDC_CHANNEL_2);
}

static void example_ledc_init(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t r = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = 4,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };

	ledc_channel_config_t g = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL_1,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = 25,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };


	ledc_channel_config_t b = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL_2,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = 26,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };



	ESP_ERROR_CHECK(ledc_channel_config(&r));
	ESP_ERROR_CHECK(ledc_channel_config(&g));
	ESP_ERROR_CHECK(ledc_channel_config(&b));
}

void app_main(void)
{
    // Set the LEDC peripheral configuration
    example_ledc_init();

	while(1){
		unsigned long t = millis();
		set_color(sin(t/1000.0f), sin(t/1500.0f), sin(t/1200.0f));
        vTaskDelay(portTICK_PERIOD_MS);
	}

    // Set duty to 50%
}
