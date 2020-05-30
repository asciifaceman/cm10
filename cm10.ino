/*
 * CM10 Modular Synth Clock
 * Written by Charles (asciifaceman) Corbett
 * Special thanks to Naftuli Kay
 * 
 */
#include <SimpleTimer.h>

// Import configuration
#include "ppqn.h"
#include "config.h"
#include "cycle.h"
#include "display.h"

// timer instantiation
SimpleTimer timer;
int count = 0;
int maxCount = 95;
bool running = false;
bool ready = false;
bool has_sync = false;
bool show_bpm = false;

unsigned long cycle_start;
unsigned long cycle_stop;

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
    pinMode(SIXTEENGTH_TRIPLET, OUTPUT);
    attachInterrupt(CLOCK_RESET, hard_reset, RISING);
#if HAS_SCREEN
        setup_display();
#endif
#if DEBUG
    Serial.begin(115200);
    Serial.println("Booted...");
#endif
}

void loop() {
    if (!ready) {
        // set up screen and get initial BPM
        ready = true;
    } else {
        if (!running) {
            Serial.println("Starting...");
            step_high();
            running = true;
        }
    }
    timer.run();
}

void step_high() {
    // get analog readings for BPM and duration
    int bpm_input = analogFilteredRead(BPM_IN);
    int duration_input = analogFilteredRead(DUR_IN);
    BPM = round(bpmFromAnalog(bpm_input));
    float duration_percent = percentageFromAnalog(duration_input);
    //unsigned long duration = bpmTickDuration(BPM * 2);
    unsigned long duration = ppqnFromBPM(BPM);


    // get SYNC in if applicable

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

if (show_bpm){
#if HAS_SCREEN
    Serial.println("updating screen..");
    display_update(BPM, has_sync);
    Serial.println("done");
#endif
}

#if DEBUG
    Serial.print("BPM: ");
    Serial.println(BPM);
    Serial.print("Start: ");
    Serial.println(cycle_start);
    Serial.print("End: ");
    Serial.println(cycle_stop);
    Serial.print("Step: ");
    Serial.println(count);
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
// the next iteration
void soft_reset() {
    Serial.println("soft reset");
    count = 0;
}

// hard reset resets the counter and forces and resets timers
void hard_reset() {
    Serial.println("hard reset");
    count = 0;
    for (int y=0; y < timer.getNumTimers(); y++) {
        timer.deleteTimer(y);
    }
    step_high();
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
