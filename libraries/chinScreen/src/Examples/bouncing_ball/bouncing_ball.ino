#include <Arduino.h>
#include "chinScreen.h"

lv_obj_t *player;
lv_obj_t *scoreLabel;
int score = 0;
bool gameOver = false;

// Create a random falling shape
void spawnShape() {
  if (gameOver) return;

  int shapeType = random(0, 3); // 0=rect, 1=circle, 2=triangle
  lv_obj_t *shape = nullptr;

  if (shapeType == 0) {
    shape = chinScreen_rectangle("red", "white", 30, 30, "top", "center");
  } else if (shapeType == 1) {
    shape = chinScreen_circle("blue", "white", 15, "top", "center");
  } else {
    shape = chinScreen_triangle("yellow", "white", 30, 30, "top", "center");
  }

  // Random X
  lv_obj_set_x(shape, random(20, 200));

  // Animate downwards
  chinScreen_animate(shape, ANIM_MOVE_Y, 0, 240, 3000, false);

  // Check collision after animation
  lv_timer_t *timer = lv_timer_create([](lv_timer_t *t) {
    lv_obj_t *s = (lv_obj_t*) t->user_data;
    if (!s || gameOver) return;

    int sx = lv_obj_get_x(s);
    int sy = lv_obj_get_y(s);
    int px = lv_obj_get_x(player);
    int py = lv_obj_get_y(player);

    if (abs(sx - px) < 30 && abs(sy - py) < 30) {
      chinScreen_text("GAME OVER!", 80, 120, "red", "large");
      gameOver = true;
    } else if (sy >= 220) {
      score++;
      char buf[20];
      sprintf(buf, "Score: %d", score);
      lv_label_set_text(scoreLabel, buf);
    }

    lv_obj_del(s);
    lv_timer_del(t);
  }, 3000, shape);
}

void setup() {
  Serial.begin(115200);
  init_display();
  chinScreen_clear();
  chinScreen_background_solid("black");

  // Player rectangle at bottom
  player = chinScreen_rectangle("green", "white", 40, 20, "bottom", "center");
  lv_obj_set_y(player, 220);

  // Score
  scoreLabel = lv_label_create(lv_scr_act());
  lv_label_set_text(scoreLabel, "Score: 0");
  lv_obj_set_style_text_color(scoreLabel, getColorByName("white"), LV_PART_MAIN);
  lv_obj_align(scoreLabel, LV_ALIGN_TOP_MID, 0, 5);

  // Spawn new shapes every 2s
  lv_timer_create([](lv_timer_t *t) {
    spawnShape();
  }, 2000, NULL);
}

void loop() {
  lv_timer_handler();
  delay(5);
}
