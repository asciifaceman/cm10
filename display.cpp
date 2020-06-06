/*
 * OLED Display
 */
#include <Arduino.h>

#include "config.h"
#include "display.h"

#if HAS_SCREEN
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"


Adafruit_SSD1306 display(0);

/*
 * setup_display does initial configuration and wipe of the display
 * called in setup
 */
void setup_display() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDR);
  Wire.setClock(400000);
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(2);
  display.display();
}

/*
 * clear_display is just a basic wipe
 */
void clear_display(){
  display.clearDisplay();
  display.display();
}

/*
 * small_text sets size to 1 and prints text at x,y
 */
void small_text(String text, int x, int y) {
  display.setCursor(x, y);
  display.setTextSize(1);
  display.print(text);
}

/*
 * large_text sets size to 2 and prints text at x,y
 */
void large_text(String text, int x, int y) {
  display.setCursor(x, y);
  display.setTextSize(2);
  display.print(text);  
}

/*
 * display_status updates the screen with sync status, bpm, duration, and pulse duration
 */
void display_status(bool active_sync, int bpm, unsigned long duration, float duration_perc) {
  display.clearDisplay();
  // Sync on or off
  if (active_sync) {
    small_text("SYNC> ON", 0, 0);
  } else {
    small_text("SYNC> OFF", 0, 0);
  }

  // BPM display
  String bpm_display = "BPM: ";
  bpm_display.concat(String(bpm, DEC));
  large_text(bpm_display, 16, 10);
  

  // Step Duration
  String duration_display = "Step: ";
  String string_duration = String(duration, DEC);
  duration_display.concat(string_duration);
  duration_display.concat("ms");
  small_text(duration_display, 64, 0);

  // PW Duration
  String pulse_duration = "PW: ";
  String string_duration_perc = "";
  float temp_perc = duration_perc*100;
  //int perc = round(temp_perc);
  
  string_duration_perc.concat(String(temp_perc, DEC));
  pulse_duration.concat(string_duration_perc);
  pulse_duration.concat("%");
  small_text(pulse_duration, 72, 25);

  // update
  display.display();
}
