/*
 * CM10Clock Configuration
 * 
 */
#pragma once

// debug mode
#define DEBUG 1

// IO Disabled (no pots connected)
#define NOIO 0

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
#define HAS_SCREEN true
#define SCREEN_ADDR 0x3c

// Port Registers
#define BPM_IN A0
#define DUR_IN A1

#define CLOCK_RESET 2
#define SYNC_IN 4
#define SYNC_OUT 5

#define WHOLE_NOTE 6
#define HALF_NOTE 7
#define TRIPLET 8
#define QUARTER_NOTE 9
#define QUARTER_TRIPLET 10
#define EIGTH_NOTE 11
#define SIXTEENTH_NOTE 12
#define SIXTEENTH_TRIPLET 13

/*
* D2 (interrupt) - Clock Reset (no reboot)
* D3 (interrupt)
* D4 (T) - SYNC IN
* D5 (T) - SYNC OUT
* D6 (1/1) - Whole Note
* D7 (1/2) - Half note
* D8 (1/3) - Triplet
* D9 (1/4) - Quarter Note
* D10 (1/4-3) - Quarter Triplet
* D11 (1/8) - Eigth Note
* D12 (1/16) - Sixteenth Note
* D13 (1/16-3) - Sixteenth Triplet
*/
