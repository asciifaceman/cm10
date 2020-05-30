/*
 * Pulses Per Quarter Note and BPM Calculations
 * Uses midi specification of 24PPQN
 */
#include <arduino.h>

#include "ppqn.h"
#include "config.h"

// bpmDifference returns the difference 
// between the min and max BPM values
int bpmDifference() {
    return MAX_BPM - MIN_BPM;
}

// bpmFromAnalog calculates BPM based on a % between min and max bpm
// from analog input
float bpmFromAnalog (int value) {
    return (float((float) value / ANALOG_MAX) * (bpmDifference())) + MIN_BPM;
}

// percentageFromAnalog calculates an arbitrary percentage
// against the analog curve from analog inputs
float percentageFromAnalog (int value) {
    return abs(((float)value - (BIAS * 2))) / 1023;
}

// ppqnFromBPM returns the pulses per quarter note
float ppqnFromBPM (int bpm) {
    return ONESECOND / (bpm * ppqn);
}

// bpmFromPPQN returns the BPM from an inputted ppqn from sync
// input using MIDI PPQN
int bpmFromPPQN (float readppqn) {
    return abs(ONESECOND / (readppqn * ppqn));
}

// bpmTickDuration returns the ms per pulse 
// but this may be deprecated as its not ppqn
float bpmTickDuration (int bpm) {
    return ONESECOND / bpm;
}