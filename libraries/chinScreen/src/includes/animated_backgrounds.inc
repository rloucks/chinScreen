// Smooth Animated Gradient Functions for chinScreen
// Replace your animated_backgrounds.inc with this version

/////////////////////////////////////////////////////////////
// Smooth Animated Gradient System
/////////////////////////////////////////////////////////////

// Animation state structure
struct SmoothAnimatedGradientState {
    uint8_t current_frame;
    uint8_t total_frames;
    uint8_t animation_type;
    bool is_running;
    chinScreen_timer_t timer;
    uint32_t speed_ms;
    float progress;  // 0.0 to 1.0 for smooth interpolation
};

static SmoothAnimatedGradientState smooth_anim_state = {0, 60, 0, false, nullptr, 50, 0.0f};

// Animation types
enum {
    SMOOTH_RAINBOW_CYCLE = 0,
    SMOOTH_RAINBOW_SHIFT,
    SMOOTH_BREATHING,
    SMOOTH_WAVE,
    SMOOTH_AURORA,
    SMOOTH_FIRE_FLICKER
};

/////////////////////////////////////////////////////////////
// Color interpolation function
/////////////////////////////////////////////////////////////
lv_color_t interpolate_color(lv_color_t color1, lv_color_t color2, float t) {
    if (t <= 0.0f) return color1;
    if (t >= 1.0f) return color2;
    
    // Extract RGB components (works with LVGL's color system)
    uint8_t r1 = (color1.full >> 11) & 0x1F;  // 5-bit red
    uint8_t g1 = (color1.full >> 5) & 0x3F;   // 6-bit green  
    uint8_t b1 = color1.full & 0x1F;          // 5-bit blue
    
    uint8_t r2 = (color2.full >> 11) & 0x1F;
    uint8_t g2 = (color2.full >> 5) & 0x3F;
    uint8_t b2 = color2.full & 0x1F;
    
    // Interpolate
    uint8_t r = r1 + (float)(r2 - r1) * t;
    uint8_t g = g1 + (float)(g2 - g1) * t;
    uint8_t b = b1 + (float)(b2 - b1) * t;
    
    // Convert back to LVGL color (scale to full 8-bit)
    return lv_color_make((r * 255) / 31, (g * 255) / 63, (b * 255) / 31);
}

/////////////////////////////////////////////////////////////
// Generate smooth rainbow color at position
/////////////////////////////////////////////////////////////
lv_color_t get_rainbow_color(float position) {
    // Position 0.0 to 1.0 maps to full rainbow
    position = position - (int)position;  // Keep fractional part
    if (position < 0) position += 1.0f;
    
    float hue = position * 360.0f;  // 0-360 degrees
    
    // Simple HSV to RGB conversion
    float c = 1.0f;  // Full saturation and value
    float x = 1.0f - abs(fmod(hue / 60.0f, 2.0f) - 1.0f);
    
    float r, g, b;
    
    if (hue < 60) { r = c; g = x; b = 0; }
    else if (hue < 120) { r = x; g = c; b = 0; }
    else if (hue < 180) { r = 0; g = c; b = x; }
    else if (hue < 240) { r = 0; g = x; b = c; }
    else if (hue < 300) { r = x; g = 0; b = c; }
    else { r = c; g = 0; b = x; }
    
    return lv_color_make((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255));
}

/////////////////////////////////////////////////////////////
// Simple fmod function for ESP32
/////////////////////////////////////////////////////////////
float simple_fmod(float x, float y) {
    return x - (int)(x / y) * y;
}

float simple_abs(float x) {
    return x < 0 ? -x : x;
}

/////////////////////////////////////////////////////////////
// Function: Stop gradient animation (DEFINED FIRST)
/////////////////////////////////////////////////////////////
void chinScreen_stop_smooth_animation() {
    if (smooth_anim_state.is_running && smooth_anim_state.timer) {
        chinScreen_timer_delete(smooth_anim_state.timer);
        smooth_anim_state.timer = nullptr;
        smooth_anim_state.is_running = false;
        Serial.println("Smooth gradient animation stopped");
    }
}

/////////////////////////////////////////////////////////////
// Smooth animation callback
/////////////////////////////////////////////////////////////
void smooth_gradient_animation_callback(void* user_data) {
    if (!smooth_anim_state.is_running) return;
    
    // Calculate smooth progress (0.0 to 1.0)
    smooth_anim_state.progress = (float)smooth_anim_state.current_frame / (float)smooth_anim_state.total_frames;
    
    switch (smooth_anim_state.animation_type) {
        case SMOOTH_RAINBOW_CYCLE:
            {
                // Smooth rainbow that shifts through the spectrum
                chinScreen_gradient_stop stops[7];
                float base_position = smooth_anim_state.progress;
                
                for (int i = 0; i < 7; i++) {
                    float color_pos = base_position + (i / 7.0f);
                    stops[i].color = get_rainbow_color(color_pos);
                    stops[i].position = (i * 100) / 6;
                }
                chinScreen_background_multi_gradient(stops, 7, "vertical");
            }
            break;
            
        case SMOOTH_RAINBOW_SHIFT:
            {
                // Horizontal rainbow that shifts position
                chinScreen_gradient_stop stops[5];
                float shift = smooth_anim_state.progress * 2.0f;  // 2 full cycles
                
                for (int i = 0; i < 5; i++) {
                    stops[i].color = get_rainbow_color((i / 4.0f) + shift);
                    stops[i].position = i * 25;  // 0, 25, 50, 75, 100
                }
                chinScreen_background_multi_gradient(stops, 5, "horizontal");
            }
            break;
            
        case SMOOTH_BREATHING:
            {
                // Breathe between blue and white
                lv_color_t dark_blue = lv_color_make(0, 0, 100);
                lv_color_t light_blue = lv_color_make(100, 150, 255);
                lv_color_t white = lv_color_make(255, 255, 255);
                
                // Use sine wave for smooth breathing
                float breath = (sin(smooth_anim_state.progress * 2 * 3.14159f) + 1.0f) / 2.0f;
                
                chinScreen_gradient_stop stops[3];
                stops[0].color = interpolate_color(dark_blue, light_blue, breath);
                stops[0].position = 0;
                stops[1].color = interpolate_color(light_blue, white, breath);
                stops[1].position = 50;
                stops[2].color = interpolate_color(white, light_blue, breath);
                stops[2].position = 100;
                
                chinScreen_background_multi_gradient(stops, 3, "vertical");
            }
            break;
            
        case SMOOTH_WAVE:
            {
                // Color wave that moves across screen
                chinScreen_gradient_stop stops[6];
                float wave_pos = smooth_anim_state.progress;
                
                for (int i = 0; i < 6; i++) {
                    float pos = (i / 5.0f) + wave_pos;
                    float intensity = (sin(pos * 4 * 3.14159f) + 1.0f) / 2.0f;
                    
                    stops[i].color = interpolate_color(
                        lv_color_make(0, 50, 100),    // Dark blue
                        lv_color_make(0, 255, 255),   // Cyan
                        intensity
                    );
                    stops[i].position = (i * 100) / 5;
                }
                chinScreen_background_multi_gradient(stops, 6, "horizontal");
            }
            break;
            
        case SMOOTH_AURORA:
            {
                // Aurora with moving colors
                chinScreen_gradient_stop stops[8];
                float time = smooth_anim_state.progress * 4;  // 4 cycles
                
                lv_color_t aurora_colors[] = {
                    lv_color_make(0, 0, 50),      // Dark blue
                    lv_color_make(0, 100, 150),   // Blue
                    lv_color_make(0, 200, 100),   // Green-blue
                    lv_color_make(100, 255, 50),  // Green
                    lv_color_make(255, 255, 100), // Yellow-green
                    lv_color_make(255, 150, 0),   // Orange
                    lv_color_make(200, 0, 100),   // Purple
                    lv_color_make(50, 0, 100)     // Dark purple
                };
                
                for (int i = 0; i < 8; i++) {
                    // Add some wave motion to colors
                    float wave = sin((time + i * 0.5f) * 3.14159f) * 0.3f + 0.7f;
                    
                    uint8_t r = (aurora_colors[i].full >> 11) & 0x1F;
                    uint8_t g = (aurora_colors[i].full >> 5) & 0x3F;
                    uint8_t b = aurora_colors[i].full & 0x1F;
                    
                    stops[i].color = lv_color_make(
                        (r * 255 / 31) * wave,
                        (g * 255 / 63) * wave,
                        (b * 255 / 31) * wave
                    );
                    stops[i].position = (i * 100) / 7;
                }
                chinScreen_background_multi_gradient(stops, 8, "vertical");
            }
            break;
            
        case SMOOTH_FIRE_FLICKER:
            {
                // Fire effect with flickering
                chinScreen_gradient_stop stops[5];
                float flicker = sin(smooth_anim_state.progress * 8 * 3.14159f) * 0.2f + 0.8f;
                
                stops[0].color = lv_color_make(50 * flicker, 0, 0);           // Dark red
                stops[0].position = 0;
                stops[1].color = lv_color_make(150 * flicker, 50 * flicker, 0); // Red-orange
                stops[1].position = 25;
                stops[2].color = lv_color_make(255 * flicker, 100 * flicker, 0); // Orange
                stops[2].position = 50;
                stops[3].color = lv_color_make(255, 200 * flicker, 0);        // Yellow
                stops[3].position = 75;
                stops[4].color = lv_color_make(255, 255, 200 * flicker);      // White-yellow
                stops[4].position = 100;
                
                chinScreen_background_multi_gradient(stops, 5, "vertical");
            }
            break;
    }
    
    smooth_anim_state.current_frame++;
    if (smooth_anim_state.current_frame >= smooth_anim_state.total_frames) {
        smooth_anim_state.current_frame = 0;  // Loop back
    }
}

/////////////////////////////////////////////////////////////
// Function: Start smooth rainbow cycle
/////////////////////////////////////////////////////////////
void chinScreen_animate_smooth_rainbow(uint32_t speed_ms = 50) {
    chinScreen_stop_smooth_animation();
    
    smooth_anim_state.animation_type = SMOOTH_RAINBOW_CYCLE;
    smooth_anim_state.current_frame = 0;
    smooth_anim_state.total_frames = 120;  // 120 frames for smooth cycle
    smooth_anim_state.speed_ms = speed_ms;
    smooth_anim_state.is_running = true;
    
    smooth_anim_state.timer = chinScreen_timer_create(smooth_gradient_animation_callback, speed_ms, nullptr);
    Serial.printf("Started smooth rainbow animation at %dms\n", speed_ms);
}

/////////////////////////////////////////////////////////////
// Function: Start smooth rainbow shift
/////////////////////////////////////////////////////////////
void chinScreen_animate_smooth_shift(uint32_t speed_ms = 80) {
    chinScreen_stop_smooth_animation();
    
    smooth_anim_state.animation_type = SMOOTH_RAINBOW_SHIFT;
    smooth_anim_state.current_frame = 0;
    smooth_anim_state.total_frames = 100;
    smooth_anim_state.speed_ms = speed_ms;
    smooth_anim_state.is_running = true;
    
    smooth_anim_state.timer = chinScreen_timer_create(smooth_gradient_animation_callback, speed_ms, nullptr);
    Serial.printf("Started smooth shift animation at %dms\n", speed_ms);
}

/////////////////////////////////////////////////////////////
// Function: Start smooth breathing
/////////////////////////////////////////////////////////////
void chinScreen_animate_smooth_breathing(uint32_t speed_ms = 100) {
    chinScreen_stop_smooth_animation();
    
    smooth_anim_state.animation_type = SMOOTH_BREATHING;
    smooth_anim_state.current_frame = 0;
    smooth_anim_state.total_frames = 60;  // One breath cycle
    smooth_anim_state.speed_ms = speed_ms;
    smooth_anim_state.is_running = true;
    
    smooth_anim_state.timer = chinScreen_timer_create(smooth_gradient_animation_callback, speed_ms, nullptr);
    Serial.printf("Started smooth breathing animation at %dms\n", speed_ms);
}

/////////////////////////////////////////////////////////////
// Function: Start smooth wave
/////////////////////////////////////////////////////////////
void chinScreen_animate_smooth_wave(uint32_t speed_ms = 60) {
    chinScreen_stop_smooth_animation();
    
    smooth_anim_state.animation_type = SMOOTH_WAVE;
    smooth_anim_state.current_frame = 0;
    smooth_anim_state.total_frames = 80;
    smooth_anim_state.speed_ms = speed_ms;
    smooth_anim_state.is_running = true;
    
    smooth_anim_state.timer = chinScreen_timer_create(smooth_gradient_animation_callback, speed_ms, nullptr);
    Serial.printf("Started smooth wave animation at %dms\n", speed_ms);
}

/////////////////////////////////////////////////////////////
// Function: Start smooth aurora
/////////////////////////////////////////////////////////////
void chinScreen_animate_smooth_aurora(uint32_t speed_ms = 100) {
    chinScreen_stop_smooth_animation();
    
    smooth_anim_state.animation_type = SMOOTH_AURORA;
    smooth_anim_state.current_frame = 0;
    smooth_anim_state.total_frames = 120;
    smooth_anim_state.speed_ms = speed_ms;
    smooth_anim_state.is_running = true;
    
    smooth_anim_state.timer = chinScreen_timer_create(smooth_gradient_animation_callback, speed_ms, nullptr);
    Serial.printf("Started smooth aurora animation at %dms\n", speed_ms);
}

/////////////////////////////////////////////////////////////
// Function: Start smooth fire flicker
/////////////////////////////////////////////////////////////
void chinScreen_animate_smooth_fire(uint32_t speed_ms = 40) {
    chinScreen_stop_smooth_animation();
    
    smooth_anim_state.animation_type = SMOOTH_FIRE_FLICKER;
    smooth_anim_state.current_frame = 0;
    smooth_anim_state.total_frames = 80;
    smooth_anim_state.speed_ms = speed_ms;
    smooth_anim_state.is_running = true;
    
    smooth_anim_state.timer = chinScreen_timer_create(smooth_gradient_animation_callback, speed_ms, nullptr);
    Serial.printf("Started smooth fire animation at %dms\n", speed_ms);
}

/////////////////////////////////////////////////////////////
// Function: Change smooth animation speed
/////////////////////////////////////////////////////////////
void chinScreen_change_smooth_speed(uint32_t new_speed_ms) {
    if (smooth_anim_state.is_running && smooth_anim_state.timer) {
        chinScreen_timer_set_period(smooth_anim_state.timer, new_speed_ms);
        smooth_anim_state.speed_ms = new_speed_ms;
        Serial.printf("Smooth animation speed changed to %dms\n", new_speed_ms);
    }
}

/////////////////////////////////////////////////////////////
// Function: Check if smooth animation is running
/////////////////////////////////////////////////////////////
bool chinScreen_is_smooth_animating() {
    return smooth_anim_state.is_running;
}
