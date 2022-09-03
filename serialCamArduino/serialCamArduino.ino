#include "esp_camera.h"

#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

camera_fb_t *fb = NULL;
bool CAM_STATE = false;

void start_camera()
{
    if (!CAM_STATE)
    {
        camera_config_t config;
        config.ledc_channel = LEDC_CHANNEL_0;
        config.ledc_timer = LEDC_TIMER_0;
        config.pin_d0 = Y2_GPIO_NUM;
        config.pin_d1 = Y3_GPIO_NUM;
        config.pin_d2 = Y4_GPIO_NUM;
        config.pin_d3 = Y5_GPIO_NUM;
        config.pin_d4 = Y6_GPIO_NUM;
        config.pin_d5 = Y7_GPIO_NUM;
        config.pin_d6 = Y8_GPIO_NUM;
        config.pin_d7 = Y9_GPIO_NUM;
        config.pin_xclk = XCLK_GPIO_NUM;
        config.pin_pclk = PCLK_GPIO_NUM;
        config.pin_vsync = VSYNC_GPIO_NUM;
        config.pin_href = HREF_GPIO_NUM;
        config.pin_sscb_sda = SIOD_GPIO_NUM;
        config.pin_sscb_scl = SIOC_GPIO_NUM;
        config.pin_pwdn = PWDN_GPIO_NUM;
        config.pin_reset = RESET_GPIO_NUM;
        config.xclk_freq_hz = 20000000;
        config.pixel_format = PIXFORMAT_JPEG;
        if (psramFound())
        {
            config.frame_size = FRAMESIZE_UXGA;
            config.jpeg_quality = 10;
            config.fb_count = 2;
        }
        else
        {
            config.frame_size = FRAMESIZE_SVGA;
            config.jpeg_quality = 12;
            config.fb_count = 1;
        }
        // Camera init
        esp_err_t err = esp_camera_init(&config);
        if (err != ESP_OK)
        {
            Serial.printf("\nCamera erro %x\n", err);
            CAM_STATE = false;
        }
        else
        {
            Serial.printf("\nCamera init success\n");
            CAM_STATE = true;
        }
    }else{
        Serial.printf("\nCamera already init\n");
    }
}

void setup()
{
    Serial.begin(500000);
}

void block_sender(camera_fb_t *_fb)
{
    int start = 0;
    while (start < _fb->len)
    {
        if (Serial.available())
        {
            char c = Serial.read();
            if (c == 'r')
            {
                int end = start + 10000;
                if (end > _fb->len)
                {
                    end = _fb->len;
                }
                for (int i = start; i < end; i++)
                {
                    Serial.write(*((fb->buf) + i));
                }
                start = end;
            }
        }
    }
    Serial.print("\n\n\n\n");
}

void loop()
{
    if (Serial.available())
    {
        char c = Serial.read();
        if (c == 'q')
        {
            fb = esp_camera_fb_get();
            Serial.printf("width:%d, hight:%d, format:%d, len:%d, time:%d\n",
                          fb->width, fb->height, fb->format, fb->len, fb->timestamp);
            for (int i = 0; i < fb->len; i++)
            {
                Serial.write(*((fb->buf) + i));
            }
            Serial.print("\n\n\n\n");
            esp_camera_fb_return(fb);
            fb = NULL;
        }
        if (c == 'v')
        {
            fb = esp_camera_fb_get();
            block_sender(fb);
            esp_camera_fb_return(fb);
            fb = NULL;
        }
        if (c == 'w')
        {
            if(CAM_STATE == true){
                esp_err_t err = esp_camera_deinit();
                if (err != ESP_OK)
                {
                    Serial.printf("\nCamera deinit erro %x\n", err);
                }
                else
                {
                    Serial.printf("\nCamera deinit success\n");
                    CAM_STATE = false;
                }
            }

        }
        if (c == 'e')
        {
            start_camera();
        }
        if (c == 't')
        {
            fb = esp_camera_fb_get();
            esp_camera_fb_return(fb);
            fb = NULL;
        }
    }
    // vTaskDelay(1000);
}