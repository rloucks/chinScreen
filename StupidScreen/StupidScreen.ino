
#include <chinScreen.h>

#define LVGL_PORT_ROTATION_DEGREE               (90)

void setup()
{
    String title = "LVGL porting example";

    Serial.begin(115200);
    Serial.println(title + " start");

    Serial.println("Initialize panel device");
    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = EXAMPLE_LCD_QSPI_H_RES * EXAMPLE_LCD_QSPI_V_RES,
#if LVGL_PORT_ROTATION_DEGREE == 90
        .rotate = LV_DISP_ROT_90,
#elif LVGL_PORT_ROTATION_DEGREE == 270
        .rotate = LV_DISP_ROT_270,
#elif LVGL_PORT_ROTATION_DEGREE == 180
        .rotate = LV_DISP_ROT_180,
#elif LVGL_PORT_ROTATION_DEGREE == 0
        .rotate = LV_DISP_ROT_NONE,
#endif
    };

    bsp_display_start_with_config(&cfg);
    bsp_display_backlight_on();

    Serial.println("Create UI");
    /* Lock the mutex due to the LVGL APIs are not thread-safe */
    bsp_display_lock(0);
        /* Create simple label */
    lv_obj_t *label = lv_label_create( lv_scr_act() );
    lv_label_set_text( label, "New Test");
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );


    bsp_display_unlock();

    Serial.println(title + " end");
}

void loop()
{
    Serial.println("IDLE loop");
    delay(1000);
}
