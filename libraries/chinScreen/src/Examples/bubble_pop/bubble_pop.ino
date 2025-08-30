// chinScreen Pop the Bubbles Game
// Touch bubbles to pop them! Background changes every 10 points
// Pure chinScreen library - minimal code, maximum fun!

#include "chinScreen.h"

// Game objects
lv_obj_t* bubbles[6];
lv_obj_t* scoreLabel;

// Game state
int score = 0;
int lives = 3;
int frameCount = 0;
bool gameRunning = true;

// Bubble properties
float bubbleX[6], bubbleY[6];
float bubbleSpeedY[6];
bool bubbleActive[6];
int bubbleSize[6];
int respawnTimer[6]; // Separate timer array to avoid conflicts

// Gradient combinations for bubbles
const char* bubbleGradients[][2] = {
    {"blue", "cyan"},
    {"purple", "pink"}, 
    {"green", "lime"},
    {"red", "orange"},
    {"yellow", "white"},
    {"navy", "blue"}
};

// Background gradients that change every 10 points
const char* backgroundGradients[][3] = {
    {"black", "navy", "vertical"},
    {"purple", "blue", "horizontal"},
    {"green", "cyan", "vertical"},
    {"red", "yellow", "horizontal"},
    {"navy", "purple", "vertical"},
    {"orange", "pink", "horizontal"}
};

void setup() {
    Serial.begin(115200);
    init_display();
    
    // Start with first background
    chinScreen_background_gradient("black", "navy", "vertical");
    
    // Create score and lives display
    scoreLabel = lv_label_create(lv_scr_act());
    updateScore(); // Initialize with proper text
    lv_obj_set_style_text_color(scoreLabel, getColorByName("white"), LV_PART_MAIN);
    lv_obj_set_style_text_font(scoreLabel, FONT_LARGE, LV_PART_MAIN);
    lv_obj_set_pos(scoreLabel, 10, 10);
    
    // Initialize bubbles
    spawnAllBubbles();
    
    Serial.println("Pop the Bubbles game started!");
}

void loop() {
    if (!gameRunning) {
        delay(100);
        return;
    }
    
    frameCount++;
    
    // Update all bubbles
    for(int i = 0; i < 6; i++) {
        if (!bubbleActive[i]) {
            // Count down respawn timer
            respawnTimer[i]--;
            if (respawnTimer[i] <= 0) {
                respawnBubble(i);
                bubbleActive[i] = true;
                Serial.printf("Bubble %d respawned!\n", i);
            }
            continue;
        }
        
        // Move bubble up (bubbles float!)
        bubbleY[i] -= bubbleSpeedY[i];
        
        // Check if bubble escaped (reached top) - LOSE LIFE!
        if (bubbleY[i] < -bubbleSize[i]) {
            lives--;
            updateScore();
            Serial.printf("Bubble escaped! Lives remaining: %d\n", lives);
            
            if (lives <= 0) {
                gameOver();
                return;
            }
            
            // Respawn this bubble
            respawnBubble(i);
        } else {
            // Update bubble position
            chinScreen_set_position(bubbles[i], (int)bubbleX[i], (int)bubbleY[i]);
        }
    }
    
    // Continuously spawn new bubbles every 3 seconds
    if (frameCount % 100 == 0) {
        int randomBubble = random(0, 6);
        // Force respawn a random bubble to keep action going
        respawnBubble(randomBubble);
        bubbleActive[randomBubble] = true;
        Serial.printf("Force spawned bubble %d\n", randomBubble);
    }
    
    delay(30); // ~33 FPS for smooth bubble floating
}

// Initialize bubbles
void spawnAllBubbles() {
    for(int i = 0; i < 6; i++) {
        bubbleX[i] = random(0, 240);
        bubbleY[i] = random(420, 520); // Start just below screen (screen is 480 high)
        bubbleSpeedY[i] = random(50, 200) / 100.0; // 0.5 to 2.0 speed
        bubbleSize[i] = random(20, 50); // Random sizes
        bubbleActive[i] = true;
        respawnTimer[i] = 0;
        
        // Create bubble with gradient
        int gradIndex = random(0, 6);
        bubbles[i] = chinScreen_circle(bubbleGradients[gradIndex][0], "white", 
                                      bubbleSize[i], "top", "left");
        
        // Add gradient to bubble
        bsp_display_lock(0);
        lv_obj_set_style_bg_color(bubbles[i], getColorByName(bubbleGradients[gradIndex][0]), LV_PART_MAIN);
        lv_obj_set_style_bg_grad_color(bubbles[i], getColorByName(bubbleGradients[gradIndex][1]), LV_PART_MAIN);
        lv_obj_set_style_bg_grad_dir(bubbles[i], LV_GRAD_DIR_VER, LV_PART_MAIN);
        bsp_display_unlock();
        
        chinScreen_set_position(bubbles[i], (int)bubbleX[i], (int)bubbleY[i]);
        
        // Add click event
        lv_obj_add_event_cb(bubbles[i], bubble_popped, LV_EVENT_CLICKED, (void*)(intptr_t)i);
    }
}

// Respawn single bubble
void respawnBubble(int i) {
    // Fixed spawn range - start just below visible screen
    bubbleX[i] = random(40, 280);
    bubbleY[i] = random(480, 520); // Just below screen bottom (480)
    bubbleSpeedY[i] = random(50, 250) / 100.0;
    bubbleSize[i] = random(20, 50);
    
    // Delete old bubble completely to fix flickering
    bsp_display_lock(0);
    if (bubbles[i]) {
        lv_obj_del_async(bubbles[i]); // Async delete to prevent flicker
    }
    bsp_display_unlock();
    
    // Short delay to ensure cleanup
    delay(10);
    
    // Create fresh bubble with new gradient
    int gradIndex = random(0, 6);
    bubbles[i] = chinScreen_circle(bubbleGradients[gradIndex][0], "white", 
                                  bubbleSize[i], "top", "left");
    
    // Add gradient
    bsp_display_lock(0);
    lv_obj_set_style_bg_color(bubbles[i], getColorByName(bubbleGradients[gradIndex][0]), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(bubbles[i], getColorByName(bubbleGradients[gradIndex][1]), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(bubbles[i], LV_GRAD_DIR_VER, LV_PART_MAIN);
    bsp_display_unlock();
    
    chinScreen_set_position(bubbles[i], (int)bubbleX[i], (int)bubbleY[i]);
    
    // Add click event to new bubble
    lv_obj_add_event_cb(bubbles[i], bubble_popped, LV_EVENT_CLICKED, (void*)(intptr_t)i);
}

// Bubble popped event
void bubble_popped(lv_event_t* e) {
    if (!gameRunning) return;
    
    int bubbleIndex = (intptr_t)lv_event_get_user_data(e);
    
    // Shrink bubble to nothing
    createPopEffect(bubbleIndex);
    
    // Score points
    score++;
    updateScore();
    
    // Change background every 10 points
    if (score % 10 == 0) {
        changeBackground();
    }
    
    Serial.printf("Bubble popped! Score: %d\n", score);
}

// Change background gradient every 10 points
void changeBackground() {
    int bgIndex = (score / 10) % 6;
    chinScreen_background_gradient(backgroundGradients[bgIndex][0], 
                                 backgroundGradients[bgIndex][1], 
                                 backgroundGradients[bgIndex][2]);
    
    Serial.printf("Background changed! Level %d: %s->%s (%s)\n", 
                 score/10, backgroundGradients[bgIndex][0], 
                 backgroundGradients[bgIndex][1], backgroundGradients[bgIndex][2]);
}

// Simple shrink-to-nothing pop effect
void createPopEffect(int bubbleIndex) {
    // Animate the bubble shrinking to nothing
    int currentSize = bubbleSize[bubbleIndex] * 2; // Current diameter
    chinScreen_animate_advanced(bubbles[bubbleIndex], ANIM_SCALE, currentSize, 0, 200, false, "ease_in");
    
    // Mark as inactive and set respawn timer
    bubbleActive[bubbleIndex] = false;
    respawnTimer[bubbleIndex] = 15; // 15 frames until respawn
}

// Update score and lives display
void updateScore() {
    char scoreText[40];
    snprintf(scoreText, sizeof(scoreText), "Score: %d  Lives: %d", score, lives);
    lv_label_set_text(scoreLabel, scoreText);
}

// Game over screen
void gameOver() {
    gameRunning = false;
    
    // Clear screen with dramatic red background
    chinScreen_background_solid("red");
    
    // Game over message
    chinScreen_text("GAME OVER!", 82, 150, "white", "xlarge");
    
    // Final score
    char finalText[40];
    snprintf(finalText, sizeof(finalText), "Final Score: %d", score);
    chinScreen_text(finalText, 88, 180, "yellow", "large");
    
    // Reset button
    chinScreen_button("blue", "white", 150, 60, "Play Again", restart_game, "middle", "center");
    
    Serial.printf("Game Over! Final Score: %d\n", score);
}

// Restart the game
void restart_game(lv_event_t* e) {
    Serial.println("Restarting game...");
    
    // Reset game state
    score = 0;
    lives = 3;
    gameRunning = true;
    frameCount = 0;
    
    // Clear screen and restart
    chinScreen_clear();
    chinScreen_background_gradient("black", "navy", "vertical");
    
    // Recreate score display
    scoreLabel = lv_label_create(lv_scr_act());
    updateScore();
    lv_obj_set_style_text_color(scoreLabel, getColorByName("white"), LV_PART_MAIN);
    lv_obj_set_style_text_font(scoreLabel, FONT_LARGE, LV_PART_MAIN);
    lv_obj_set_pos(scoreLabel, 10, 10);
    
    // Respawn all bubbles
    spawnAllBubbles();
}