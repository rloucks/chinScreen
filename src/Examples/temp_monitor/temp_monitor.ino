// chinScreen Bouncing Ball Example
// Super simple animated ball that bounces around the screen

#include "chinScreen.h"

lv_obj_t* ball;
int ballX = 50, ballY = 50;
int speedX = 3, speedY = 2;
int screenWidth = 320, screenHeight = 240; // Adjust for your screen
int ballSize = 30;

void setup() {
    Serial.begin(115200);
    init_display();
    
    // Dark background with gradient
    chinScreen_background_gradient("navy", "purple", "vertical");
    
    // Create the bouncing ball
    ball = chinScreen_circle("orange", "yellow", ballSize, "top", "left");
    chinScreen_set_position(ball, ballX, ballY);
    
    // Add some sparkle with small decorative shapes
    chinScreen_circle("yellow", "white", 8, "top", "right");
    chinScreen_circle("cyan", "white", 6, "bottom", "left");
    chinScreen_circle("pink", "white", 10, "top", "left");
    
    Serial.println("Bouncing ball started!");
}

void loop() {
    // Update ball position
    ballX += speedX;
    ballY += speedY;
    
    // Bounce off walls
    if (ballX <= 0 || ballX >= screenWidth - ballSize*2) {
        speedX = -speedX;
    }
    if (ballY <= 0 || ballY >= screenHeight - ballSize*2) {
        speedY = -speedY;
    }
    
    // Move the ball
    chinScreen_set_position(ball, ballX, ballY);
    
    // Small delay for smooth animation
    delay(16); // ~60 FPS
}