/*
 * PPQN and BPM calculations
 * 
 */
#pragma once

#define ppqn 24
#define ANALOG_MIN 0
#define ANALOG_MAX 1023
#define ONESECOND 60000

float ppqnFromBPM (int);
float bpmFromAnalog (int);
float percentageFromAnalog (int);
float bpmTickDuration (int);
