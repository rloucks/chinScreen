/////////////////////////////////////////////////////////////
// Debug function
/////////////////////////////////////////////////////////////
inline void create_ui(int debugChin) {
    Serial.println("Create UI");

    bsp_display_lock(0);  
    
    if (debugChin == 1) {
        // Clear screen first to prevent overlap
        lv_obj_clean(lv_scr_act());
        
        lv_obj_t *label = lv_label_create(lv_scr_act());
        lv_label_set_text(label, "chinScreen Works!");
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    }
    
    if (debugChin == 2) {
        // Clear screen before creating new content
        lv_obj_clean(lv_scr_act());

        // Array of test colors
        lv_color_t colors[] = {
            lv_palette_main(LV_PALETTE_RED),
            lv_palette_main(LV_PALETTE_GREEN),
            lv_palette_main(LV_PALETTE_BLUE),
            lv_palette_main(LV_PALETTE_YELLOW),
            lv_palette_main(LV_PALETTE_ORANGE),
            lv_palette_main(LV_PALETTE_PURPLE),
            lv_palette_main(LV_PALETTE_CYAN),
            lv_palette_main(LV_PALETTE_GREY),
        };

        const char* names[] = {
            "Red", "Green", "Blue", "Yellow", "Orange", "Purple", "Cyan", "Grey"
        };

        // Create colored rectangles with labels
        for (int i = 0; i < 8; i++) {
            lv_obj_t *rect = lv_obj_create(lv_scr_act());
            lv_obj_set_size(rect, 80, 60);
            lv_obj_set_style_bg_color(rect, colors[i], LV_PART_MAIN);
            lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, LV_PART_MAIN);
            lv_obj_align(rect, LV_ALIGN_TOP_LEFT, (i % 4) * 85, (i / 4) * 65);

            lv_obj_t *label = lv_label_create(rect);
            lv_label_set_text(label, names[i]);
            lv_obj_center(label);
        }
    }
    
    bsp_display_unlock();
    Serial.println("UI created");
}
