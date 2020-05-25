/*
 * CM10Clock Configuration
 * 
 */
#pragma once

// debug mode
#define DEBUG 1

// IO Disabled (no pots connected)
#define NOIO 1

// Static BPM to use if NOIO
#define NOIO_BPM 120

// Static duration to use if NOIO
#define NOIO_DURATION 50

// Lowest BPM Supported
#define MIN_BPM 60

// Highest BPM Supported
#define MAX_BPM 220

// bias for pulse width, shortest pulse
#define BIAS 25

// screen
#define HAS_SCREEN false
#define SCREEN_ADDR 0x3c