// Flying Icons Screensaver - Random icons fly across the screen
// Uses your animation library for smooth movement

#define CHINSCREEN_ENABLE_ICONS
#define CHINSCREEN_ENABLE_ANIMATIONS
#define CHINSCREEN_ICON_HOME
#define CHINSCREEN_ICON_SETTINGS
#define CHINSCREEN_ICON_BACK
#define CHINSCREEN_ICON_PLAY
#define CHINSCREEN_ICON_USER
#define CHINSCREEN_ICON_SIGNAL
#define CHINSCREEN_ICON_UPLOAD
#define CHINSCREEN_ICON_VOLUME

#define LVGL_PORT_ROTATION_DEGREE 0
#include <chinScreen.h>

// Screensaver configuration
#define MAX_FLYING_ICONS 8
#define SPAWN_INTERVAL 2000  // ms between new icons
#define ICON_SPEED 3000      // ms to cross screen

struct FlyingIcon {
    lv_obj_t* obj;
    bool active;
    unsigned long spawn_time;
};

FlyingIcon flying_icons[MAX_FLYING_ICONS];
unsigned long last_spawn = 0;

// Available icons and colors for randomization
const char* available_icons[] = {"home", "settings", "back", "play", "user", "signal", "upload", "volume"};
const char* available_colors[] = {"red", "blue", "green", "yellow", "orange", "purple", "cyan", "magenta", "pink", "lime"};
const int icon_count = sizeof(available_icons) / sizeof(available_icons[0]);
const int color_count = sizeof(available_colors) / sizeof(available_colors[0]);

void setup() {
    Serial.begin(115200);
    init_display();
    
    // Initialize screensaver
    chinScreen_background_gradient("black", "navy", "vertical");
    chinScreen_text("Flying Icons", 0, 0, "white", "medium");
    
    // Initialize flying icons array
    for (int i = 0; i < MAX_FLYING_ICONS; i++) {
        flying_icons[i].obj = nullptr;
        flying_icons[i].active = false;
        flying_icons[i].spawn_time = 0;
    }
    
    Serial.println("Screensaver started!");
}

void loop() {
    update_screensaver();
    lv_timer_handler();
    delay(5);
}

void update_screensaver() {
    unsigned long current_time = millis();
    
    // Spawn new icon if it's time
    if (current_time - last_spawn > SPAWN_INTERVAL) {
        spawn_random_icon();
        last_spawn = current_time;
    }
    
    // Clean up finished icons
    cleanup_finished_icons();
}

void spawn_random_icon() {
    // Find empty slot
    int slot = find_empty_slot();
    if (slot == -1) return; // No empty slots
    
    // Random icon and color
    const char* icon_name = available_icons[random(icon_count)];
    const char* icon_color = available_colors[random(color_count)];
    
    // Random size and position
    float zoom = random(10, 40) / 100.0f; // 0.1 to 0.4x zoom
    int start_x, start_y, end_x, end_y;
    
    // Random direction (4 directions: left, right, top, bottom)
    int direction = random(4);
    switch (direction) {
        case 0: // Left to right
            start_x = -50;
            start_y = random(50, 270);
            end_x = 370;
            end_y = start_y + random(-50, 50);
            break;
        case 1: // Right to left
            start_x = 370;
            start_y = random(50, 270);
            end_x = -50;
            end_y = start_y + random(-50, 50);
            break;
        case 2: // Top to bottom
            start_x = random(50, 270);
            start_y = -50;
            end_x = start_x + random(-50, 50);
            end_y = 530;
            break;
        case 3: // Bottom to top
            start_x = random(50, 270);
            start_y = 530;
            end_x = start_x + random(-50, 50);
            end_y = -50;
            break;
    }
    
    // Create the icon
    lv_obj_t* icon = chinScreen_icon(icon_name, zoom, "middle", "center", icon_color, start_x, start_y);
    
    if (icon) {
        // Store in slot
        flying_icons[slot].obj = icon;
        flying_icons[slot].active = true;
        flying_icons[slot].spawn_time = millis();
        
        // Start movement animation
        chinScreen_animate_advanced(icon, ANIM_MOVE_X, start_x, end_x, ICON_SPEED, false, "ease_out");
        chinScreen_animate_advanced(icon, ANIM_MOVE_Y, start_y, end_y, ICON_SPEED, false, "ease_out");
        
        // Add some rotation for fun
        if (random(2)) {
            chinScreen_animate_advanced(icon, ANIM_ROTATE, 0, random(180, 720), ICON_SPEED, false, "linear");
        }
        
        // Fade effect
        if (random(3) == 0) {
            chinScreen_animate_advanced(icon, ANIM_FADE, 255, 100, ICON_SPEED/2, false, "ease_in");
        }
        
        Serial.printf("Spawned %s %s icon in slot %d\n", icon_color, icon_name, slot);
    }
}

int find_empty_slot() {
    for (int i = 0; i < MAX_FLYING_ICONS; i++) {
        if (!flying_icons[i].active) {
            return i;
        }
    }
    return -1; // No empty slots
}

void print_debug_status() {
    int active_count = 0;
    for (int i = 0; i < MAX_FLYING_ICONS; i++) {
        if (flying_icons[i].active) active_count++;
    }
    Serial.printf("Debug: %d active icons, %d total animations running\n", 
                  active_count, lv_anim_count_running());
}

void refresh_screen() {
    // Force a display refresh by recreating the background
    chinScreen_background_gradient("black", "navy", "vertical");
    
    // Optionally refresh the title text
    static bool show_title = true;
    if (show_title) {
        chinScreen_text("Flying Icons Screensaver", 160, 460, "white", "small");
    }
    show_title = !show_title; // Toggle title visibility
}

void force_cleanup_oldest() {
    // Find the oldest active icon and remove it
    int oldest_slot = -1;
    unsigned long oldest_time = ULONG_MAX;
    
    for (int i = 0; i < MAX_FLYING_ICONS; i++) {
        if (flying_icons[i].active && flying_icons[i].spawn_time < oldest_time) {
            oldest_time = flying_icons[i].spawn_time;
            oldest_slot = i;
        }
    }
    
    if (oldest_slot != -1) {
        if (flying_icons[oldest_slot].obj) {
            lv_obj_del(flying_icons[oldest_slot].obj);
            flying_icons[oldest_slot].obj = nullptr;
        }
        flying_icons[oldest_slot].active = false;
        Serial.printf("Force cleaned up oldest icon in slot %d\n", oldest_slot);
    }
}

void cleanup_finished_icons() {
    unsigned long current_time = millis();
    
    for (int i = 0; i < MAX_FLYING_ICONS; i++) {
        if (flying_icons[i].active) {
            // Check if icon should be cleaned up (animation finished + some buffer time)
            if (current_time - flying_icons[i].spawn_time > ICON_SPEED + 1000) {
                if (flying_icons[i].obj) {
                    lv_obj_del(flying_icons[i].obj);
                    flying_icons[i].obj = nullptr;
                }
                flying_icons[i].active = false;
                Serial.printf("Cleaned up icon in slot %d\n", i);
            }
        }
    }
}

// Alternative screensaver with bouncing icons
void create_bouncing_screensaver() {
    chinScreen_background_solid("black");
    
    // Create several icons that bounce around
    for (int i = 0; i < 5; i++) {
        const char* icon_name = available_icons[random(icon_count)];
        const char* icon_color = available_colors[random(color_count)];
        
        int x = random(50, 270);
        int y = random(50, 430);
        
        lv_obj_t* bouncer = chinScreen_icon(icon_name, 0.3f, "middle", "center", icon_color, x, y);
        
        if (bouncer) {
            // Bouncing animation
            chinScreen_animate_advanced(bouncer, ANIM_MOVE_X, x, x + random(-100, 100), 2000, true, "bounce");
            chinScreen_animate_advanced(bouncer, ANIM_MOVE_Y, y, y + random(-100, 100), 2500, true, "bounce");
            chinScreen_animate_advanced(bouncer, ANIM_ROTATE, 0, 360, 3000, true, "linear");
        }
    }
}

// Spiral screensaver
void create_spiral_screensaver() {
    chinScreen_background_gradient("black", "purple", "diagonal");
    
    // Create icons in a spiral pattern
    for (int i = 0; i < 6; i++) {
        const char* icon_name = available_icons[i % icon_count];
        const char* icon_color = available_colors[i % color_count];
        
        lv_obj_t* spiral_icon = chinScreen_icon(icon_name, 0.2f, "middle", "center", icon_color, 160, 240);
        
        if (spiral_icon) {
            // Calculate spiral endpoints
            float angle = i * 60; // 60 degrees apart
            int radius = 100;
            int end_x = 160 + (int)(radius * cos(angle * M_PI / 180));
            int end_y = 240 + (int)(radius * sin(angle * M_PI / 180));
            
            // Delayed start for spiral effect
            lv_timer_t* delayed_timer = lv_timer_create([](lv_timer_t* timer) {
                lv_obj_t* icon = (lv_obj_t*)timer->user_data;
                
                // Get current position
                int start_x = lv_obj_get_x(icon);
                int start_y = lv_obj_get_y(icon);
                
                // Calculate new position
                float angle = random(360) * M_PI / 180;
                int end_x = 160 + (int)(100 * cos(angle));
                int end_y = 240 + (int)(100 * sin(angle));
                
                chinScreen_animate_advanced(icon, ANIM_MOVE_X, start_x, end_x, 2000, false, "ease_in_out");
                chinScreen_animate_advanced(icon, ANIM_MOVE_Y, start_y, end_y, 2000, false, "ease_in_out");
                
                lv_timer_del(timer);
            }, i * 300, spiral_icon);
            
            lv_timer_set_repeat_count(delayed_timer, 1);
        }
    }
}