/*
 * OLED Display
 */
#include <Arduino.h>
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

#include "config.h"
#include "display.h"
#if HAS_SCREEN

Adafruit_SSD1306 display(0);

void setup_display() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDR);
  Wire.setClock(400000);
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);
  display.display();
}

void small_text(String text, int x, int y) {
  display.setCursor(x, y);
  display.setTextSize(1);
  display.print(text);
}

void update_display(char top[5]) {
  display.setCursor(0, 0);
  display.println(top);
  //display.setCursor(0, 16);
  //display.print(bottom);
  display.display();
}

//char[128] create_display_line(){
//  char[128] line;
//  return line;
//}

void display_update (int bpm, bool active_sync) {
  display.setTextColor(WHITE, BLACK);
  if (active_sync) {
    small_text("SYNC> ON", 0, 0);
  } else {
    small_text("SYNC> OFF", 0, 0);
  }
  
  //small_text("CM10 CLOCK", 64, 0);

  display.setCursor(16, 16);
  display.setTextSize(2);

  display.print(String(bpm, DEC));
  
  display.print(" BPM");
  display.display();
}

#endif
