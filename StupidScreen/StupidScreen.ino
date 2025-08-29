
#define LVGL_PORT_ROTATION_DEGREE 0    // set the inital rotation. Default landscape is 90
#include <chinScreen.h>


void setup() {
    String title = "LVGL porting example";
    Serial.begin(115200);
    Serial.println(title + " start");

    init_display();   // from chinScreen
    create_ui(1);      // from chinScreen || 0 = no init text, 1 = init text

    Serial.println(title + " end");
}

void loop() {
    Serial.println("IDLE loop");
    delay(1000);
}