/*
 * CM10 Modular Synth Clock
 * Written by Charles (asciifaceman) Corbett
 * 
 * Special thanks to Naftuli Kay & Karl Q. for teaching me bitbashing
 * 
 */
#include <SimpleTimer.h>

// Import configuration
#include "ppqn.h"
#include "config.h"
#include "cycle.h"
#include "display.h"
#include "sync.h"

// timer instantiation
SimpleTimer timer;
int count = 0;
int maxCount = 95;
bool running = false;
bool ready = false;
bool halted = false;
bool has_sync = false;
bool screen_enabled = true;

unsigned long cycle_start;
unsigned long cycle_stop;
unsigned long duration;
float duration_percent;

static uint32_t lastStop = 0;

// sync in
unsigned long this_sync_pulse;
unsigned long last_sync_pulse;
int sync_period;
unsigned long max_pulse_latency = 400; // after this turn sync off

int BPM;

/*
 * Global Definitions
 */
const int analogReadCount = 10;

// Bootup configuration
void setup(){
    pinMode(SYNC_IN, INPUT);
    pinMode(SYNC_OUT, OUTPUT);
    pinMode(WHOLE_NOTE, OUTPUT);
    pinMode(HALF_NOTE, OUTPUT);
    pinMode(TRIPLET, OUTPUT);
    pinMode(QUARTER_NOTE, OUTPUT);
    pinMode(QUARTER_TRIPLET, OUTPUT);
    pinMode(EIGTH_NOTE, OUTPUT);
    pinMode(SIXTEENTH_NOTE, OUTPUT);
    pinMode(SIXTEENTH_TRIPLET, OUTPUT);

    // reset - on rising pause the clock and enable display
    // reset - on falling reset the clock to 0 and start it again
 
    attachInterrupt(digitalPinToInterrupt(CLOCK_RESET), display_interrupt, RISING);
    attachInterrupt(digitalPinToInterrupt(CLOCK_IN), accept_sync_pulse, RISING);

#if HAS_SCREEN
        setup_display();
#endif
    ready = true;
#if DEBUG
    Serial.begin(115200);
    Serial.println("Booted...");
#endif
}

void accept_sync_pulse() {
  last_sync_pulse = this_sync_pulse;
  this_sync_pulse = millis();
  if (this_sync_pulse > last_sync_pulse){
    sync_period = round(this_sync_pulse - last_sync_pulse);  
  }
}

void loop() {
    if (!ready) {
        if (halted){
          if (screen_enabled) {
            Serial.println("System paused for screen updates");
            // display screen data for setting updates feedback loop
            display_screen_data();
          } 
        }
    } else {
        if (!running) {
            if (!halted){
              Serial.println("Starting...");
              clear_display();
              step_high();
              running = true;
            }
        }
    }
    timer.run();
}

void step_high() {
    if (halted){
      return;
    }

    // get analog readings and cast them
    read_analog_input();

    // TODO

    // determine cycle rate for start/stop
    cycle_start = duration;
    cycle_stop = (unsigned long) (duration * duration_percent);

    // set the timers
    timer.setTimeout(cycle_start, step_high);
    timer.setTimeout(cycle_stop, step_low);

    // flip bits yo
    StepData state = read_step_bytes(count);
    PORTD = state.D;
    PORTB = state.B;

#if DEBUG
    Serial.print("BPM: ");
    Serial.println(BPM);
    Serial.print("Start: ");
    Serial.println(cycle_start);
    Serial.print("End: ");
    Serial.println(cycle_stop);
    Serial.print("Step: ");
    Serial.println(count);
    Serial.print("PW: ");
    Serial.print(duration_percent);
    Serial.println("%");
    Serial.print("Sync: ");
    Serial.print(this_sync_pulse);
    Serial.print(" - ");
    Serial.print(last_sync_pulse);
    Serial.print(" = ");
    Serial.println(sync_period);
    Serial.print("Sync: ");
    Serial.println(has_sync);
#endif
}

void read_analog_input(){
    // get analog readings for BPM and duration
    int bpm_input = analogFilteredRead(BPM_IN);
    int duration_input = analogFilteredRead(DUR_IN);

    // determine bpm and duration percentage
    BPM = round(bpmFromAnalog(bpm_input));
    duration_percent = percentageFromAnalog(duration_input);
    duration = ppqnFromBPM(BPM);

    unsigned long difference = this_sync_pulse - last_sync_pulse;
    if (difference > max_pulse_latency) {
      has_sync = false;
    } else {
      if (!has_sync){
        kill_timers();
        soft_reset();
        has_sync = true;
      }
    }

    // get SYNC in if applicable
    if (has_sync) {
      integrate_sync();
    }

}

void integrate_sync(){
  // this may cause problems after 20 minutes when the millis() resets but I'll make it better once it works
  duration = sync_period;
  BPM = round(bpmFromPPQN(sync_period));
}

void display_screen_data() {
#if HAS_SCREEN
    // Update screen with details
    Serial.println("updating screen..");
    read_analog_input();
    display_status(has_sync, BPM, duration, duration_percent);
    Serial.println("done");
#endif
}

void step_low() {
    // unflip bits yo
    PORTD &= B00000000;
    PORTB &= B00000000;

    count++;
    if (count >= maxCount) {
        soft_reset();
    }
}

// soft reset resets the counter and lets the timeouts manage
// the next iteration - this would be a non-interrupt lazy operation
void soft_reset() {
    Serial.println("soft reset");
    count = 0;
}

// display_interrupt fires on a HIGH signal to interrupt pin D2
// it halts the clock, kills timers, and activates the display
void display_interrupt() {
  // Software debounce
  if ( millis() - lastStop < 200) {
    return;
  }
  lastStop = millis();
  
  if (halted){
#if DEBUG
    Serial.println("Releasing halt");
#endif
    halted = false;
    ready = true;
    screen_enabled = false;
    soft_reset();
  } else {
#if DEBUG
    Serial.println("Halted counter to display data");
#endif
    halted = true;
    ready = false;
    running = false;
    kill_timers();
    screen_enabled = true;
  }
}

// kill_timers iterates and deletes the running timers
void kill_timers() {
    Serial.println("asked to kill timers");
    if (timer.getNumTimers() > 0){
      Serial.println("killing timers");
      for (int y=0; y < timer.getNumTimers(); y++) {
          timer.deleteTimer(y);
      }
    } else {
      Serial.println("no timers to kill");
    }
}

// analogFilteredRead samples from analogReadCount number
// of reads of the passed port and returns an average
int analogFilteredRead(int port) {
    if (NOIO) {
        return 1023/2;
    }
    int aggregate = 0;
    for (int i = 0; i < analogReadCount; i++){
        aggregate += analogRead(port);
    }
    return (int) aggregate / analogReadCount;
}
