/*
Code for weather Monitoring System to get data from different sensons and diplay it on
Smart IOT based on ESP32S3 by playwithcircuit.com
*/

#include "esp_check.h"
#include "esp_display_panel.hpp"
#include "esp_lib_utils.h"
#include "lvgl.h"
#include "lvgl_v8_port.h"
#include "sensors.h"

using namespace esp_panel::drivers;
using namespace esp_panel::board;


// extern the png images structures which are present in the independent c files
LV_IMG_DECLARE(temp);
LV_IMG_DECLARE(humidity);
LV_IMG_DECLARE(pressure);
LV_IMG_DECLARE(altitude);
LV_IMG_DECLARE(rain);
LV_IMG_DECLARE(light);


static char TAG[100];

// global character buffer 
char buf[32];
E_STATUS eStatus;

// global label pointer
static lv_obj_t *temp_value_label;      
static lv_obj_t *humidity_value_label;
static lv_obj_t *pressure_value_label;
static lv_obj_t *altitude_value_label;
static lv_obj_t *rainy_label;
static lv_obj_t *light_label;


// global value variables
float temp_value = 0;
unsigned char humidity_value = 0;
float pressure_value = 0;
unsigned int altitude_value = 0;
bool bIsRaining = false;
bool bIsDay = false;


// global card size
int card_w = 96;
int card_h = 100;

// static function for creating cards
static void create_card(lv_obj_t * parent,
                        int x,
                        int y,
                        int w,
                        int h,
                        const lv_img_dsc_t * icon,
                        const char * title,
                        const char * value,
                        lv_color_t border_color,
                        lv_obj_t ** value_label_out
                       )
{
    // card creation
    lv_obj_t * card = lv_obj_create(parent);

    lv_obj_set_size(card, w, h);

    lv_obj_set_pos(card, x, y);

    lv_obj_clear_flag(card,
                      LV_OBJ_FLAG_SCROLLABLE);

    // setting style
    lv_obj_set_style_radius(card, 12, 0);

    lv_obj_set_style_bg_color(card,
                              lv_color_hex(0xFFFFFF),
                              0);

    lv_obj_set_style_border_width(card,
                                  2,
                                  0);

    lv_obj_set_style_border_color(card,
                                  border_color,
                                  0);

    lv_obj_set_style_shadow_width(card,
                                  15,
                                  0);

    lv_obj_set_style_shadow_opa(card,
                                LV_OPA_20,
                                0);

    // creating image object
    lv_obj_t * img = lv_img_create(card);

    lv_img_set_src(img, icon);

    lv_obj_align(img,
                 LV_ALIGN_BOTTOM_MID,
                 0,
                 -20);


    // creating value label object
    lv_obj_t * value_label = lv_label_create(card);

    lv_label_set_text(value_label,
                      value);

    lv_obj_set_style_text_font(value_label,
                               &lv_font_montserrat_18,
                               0);

    lv_obj_set_style_text_color(value_label,
                                lv_color_hex(0x111111),
                                0);

    lv_obj_align(value_label,
                 LV_ALIGN_CENTER,
                 0,
                 30);

    // update out pointers for label values
    *value_label_out = value_label;            
}

/*----------------------------------------------------------
    MAIN
----------------------------------------------------------*/
extern "C" void app_main(void)
{
    E_STATUS eRet_Status = eSTATUS_OK;
    Board *board = new Board();

    board->init();

    board->begin();

    lvgl_port_init(board->getLCD(), board->getTouch());

    /* Landscape */
    lv_disp_set_rotation(lv_disp_get_default(), LV_DISP_ROT_270);

    eRet_Status = BME280_SensorInit();
    if(eRet_Status != eSTATUS_OK)
    {
        ESP_LOGE(TAG, "Failed to Initialize BME280 Sensor");
        while(1){
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }

    eRet_Status = ADC_Init();
    if(eRet_Status != eSTATUS_OK)
    {
        ESP_LOGE(TAG, "Failed to Initialize ADC for Ranin Sensor/LDR ");
        while(1){
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }

    lvgl_port_lock(-1);

    /*------------------------------------------------------
        SCREEN
    ------------------------------------------------------*/
    lv_obj_t * scr = lv_scr_act();

    lv_obj_set_style_bg_color(scr,
                              lv_color_hex(0xF3F4F6),
                              0);

    /*------------------------------------------------------
        HEADER
    ------------------------------------------------------*/
    lv_obj_t * header = lv_label_create(scr);

    lv_label_set_text(header,
                      "Weather Dashboard");

    lv_obj_set_style_text_font(header,
                               &lv_font_montserrat_20,
                               0);

    lv_obj_set_style_text_color(header,
                                lv_color_hex(0x111111),
                                0);

    lv_obj_align(header,
                 LV_ALIGN_TOP_MID,
                 0,
                 5);



    // first row
    create_card(scr,
                6,
                35,
                card_w,
                card_h,
                &temp,
                "Temperature",
                "27.0 C",
                lv_palette_main(LV_PALETTE_RED),
                &temp_value_label);

    create_card(scr,
                111,
                35,
                card_w,
                card_h,
                &humidity,
                "Humidity",
                "65 %",
                lv_palette_main(LV_PALETTE_BLUE),
                &humidity_value_label);

    create_card(scr,
                216,
                35,
                card_w,
                card_h,
                &pressure,
                "Pressure",
                "1008 hPa",
                lv_palette_main(LV_PALETTE_PURPLE),
                &pressure_value_label);

    // second row
    create_card(scr,
                6,
                140,
                card_w,
                card_h,
                &altitude,
                "Altitude",
                "125 m",
                lv_palette_main(LV_PALETTE_GREEN),
                &altitude_value_label);

    create_card(scr,
                111,
                140,
                card_w,
                card_h,
                &rain,
                "Rain",
                "Dry",
                lv_palette_main(LV_PALETTE_CYAN),
                &rainy_label);

    create_card(scr,
                216,
                140,
                card_w,
                card_h,
                &light,
                "Light",
                "Day",
                lv_palette_main(LV_PALETTE_ORANGE),
                &light_label);

    lvgl_port_unlock();
    while (1)
    {
        eStatus =  getBME280Data(&temp_value, &humidity_value, &pressure_value, &altitude_value);
        if(eStatus == eSTATUS_OK)
        {
            sprintf(buf, "%.1f C",temp_value);
            lvgl_port_lock(-1);
            lv_label_set_text(temp_value_label, buf);
            lvgl_port_unlock();

            sprintf(buf, "%u %%",humidity_value);
            lvgl_port_lock(-1);
            lv_label_set_text(humidity_value_label, buf);
            lvgl_port_unlock();

            sprintf(buf, "%.1f hPa",pressure_value);
            lvgl_port_lock(-1);
            lv_label_set_text(pressure_value_label, buf);
            lvgl_port_unlock();

            sprintf(buf, "%u m",altitude_value);
            lvgl_port_lock(-1);
            lv_label_set_text(altitude_value_label, buf);
            lvgl_port_unlock();
        }
        else
        {
            lvgl_port_lock(-1);
            lv_label_set_text(temp_value_label, "ERROR");
            lvgl_port_unlock();

            lvgl_port_lock(-1);
            lv_label_set_text(humidity_value_label, "ERROR");
            lvgl_port_unlock();

            lvgl_port_lock(-1);
            lv_label_set_text(pressure_value_label, "ERROR");
            lvgl_port_unlock();

            lvgl_port_lock(-1);
            lv_label_set_text(altitude_value_label, "ERROR");
            lvgl_port_unlock();
        }

        eStatus =  getRainStatus(&bIsRaining);
        if(eStatus == eSTATUS_OK)
        {
            sprintf(buf,"%s", bIsRaining ? "Rainy" : "Dry");
            lvgl_port_lock(-1);
            lv_label_set_text(rainy_label, buf);
            lvgl_port_unlock();
        }
        else
        {
            lvgl_port_lock(-1);
            lv_label_set_text(rainy_label, "ERROR");
            lvgl_port_unlock();
        }

        eStatus =  getLightStatus(&bIsDay);
        if(eStatus == eSTATUS_OK)
        {
            sprintf(buf,"%s", bIsDay ? "Day" : "Night");
            lvgl_port_lock(-1);
            lv_label_set_text(light_label, buf);
            lvgl_port_unlock();
        }
        else
        {
            lvgl_port_lock(-1);
            lv_label_set_text(light_label, "ERROR");
            lvgl_port_unlock();
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
 