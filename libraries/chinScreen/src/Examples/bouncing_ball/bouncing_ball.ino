// chinScreen Bouncing Ball Example
// Super simple animated ball that bounces around the screen

#include "chinScreen.h"

lv_obj_t* ball;
lv_obj_t* particles[3]; // Trail particles that follow the ball
int ballX = 160, ballY = 240; // Start in center
float speedX = 4.5, speedY = 3.2; // Use float for smoother random changes
int screenWidth = 320, screenHeight = 480; // Vertical screen
// int screenWidth = 480, screenHeight = 320; // Horizontal screen
int ballSize = 30;

// Particle trail positions with proper spacing
float particleX[3], particleY[3];
int frameCount = 0;

void setup() {
    Serial.begin(115200);
    init_display();
    
    // Dark background with gradient
    chinScreen_background_gradient("navy", "purple", "vertical");
    
    // Create the bouncing ball
    ball = chinScreen_circle("orange", "yellow", ballSize, "top", "left");
    chinScreen_set_position(ball, ballX, ballY);
    
    // Create particle trail - different sizes and colors
    particles[0] = chinScreen_circle("red", "red", 12, "top", "left");      // Closest to ball
    particles[1] = chinScreen_circle("yellow", "yellow", 8, "top", "left");  // Middle
    particles[2] = chinScreen_circle("white", "white", 5, "top", "left");    // Farthest
    
    // Initialize particle positions with proper spacing behind ball
    for(int i = 0; i < 3; i++) {
        particleX[i] = ballX - (i + 1) * 20; // Space them 20 pixels apart
        particleY[i] = ballY - (i + 1) * 15; // Offset vertically too
        chinScreen_set_position(particles[i], (int)particleX[i], (int)particleY[i]);
    }
    
    // Add some random to initial speed
    speedX += random(-200, 200) / 100.0; // Add -2 to +2 random
    speedY += random(-200, 200) / 100.0;
    
    Serial.println("Random bouncing ball with trail started!");
    Serial.printf("Screen: %dx%d\n", screenWidth, screenHeight);
}

void loop() {
    frameCount++;
    
    // Store previous ball position for smooth trail
    float prevBallX = ballX;
    float prevBallY = ballY;
    
    // Update ball position
    ballX += speedX;
    ballY += speedY;
    
    // Bounce off walls with MUCH more randomness
    if (ballX <= ballSize || ballX >= screenWidth - ballSize) {
        speedX = -speedX;
        // Big random changes to make it truly unpredictable
        speedX += random(-300, 300) / 100.0; // Add -3 to +3 random
        speedY += random(-200, 200) / 100.0; // Add -2 to +2 random
        // Keep speeds reasonable but allow variety
        speedX = constrain(speedX, -8, 8);
        speedY = constrain(speedY, -8, 8);
        Serial.println("Side bounce - randomized!");
    }
    
    if (ballY <= ballSize || ballY >= screenHeight - ballSize) {
        speedY = -speedY;
        // Big random changes here too
        speedX += random(-200, 200) / 100.0;
        speedY += random(-300, 300) / 100.0;
        speedX = constrain(speedX, -8, 8);
        speedY = constrain(speedY, -8, 8);
        Serial.println("Top/bottom bounce - randomized!");
    }
    
    // Update particle trail every frame for smooth following
    // Move particles toward previous positions in chain
    for(int i = 2; i >= 0; i--) {
        if (i == 0) {
            // First particle follows ball with delay
            particleX[0] += (prevBallX - particleX[0]) * 0.3; // 30% catch-up
            particleY[0] += (prevBallY - particleY[0]) * 0.3;
        } else {
            // Other particles follow the previous particle
            particleX[i] += (particleX[i-1] - particleX[i]) * 0.4; // 40% catch-up
            particleY[i] += (particleY[i-1] - particleY[i]) * 0.4;
        }
        
        // Update particle position on screen
        chinScreen_set_position(particles[i], (int)particleX[i], (int)particleY[i]);
    }
    
    // Move the ball
    chinScreen_set_position(ball, ballX, ballY);
    
    // Add tiny random variations every so often to keep it interesting
    if (frameCount % 120 == 0) { // Every 2 seconds at 60fps
        speedX += random(-50, 50) / 100.0;
        speedY += random(-50, 50) / 100.0;
        Serial.println("Random speed variation added!");
    }
    
    // Small delay for smooth animation
    delay(16); // ~60 FPS
}