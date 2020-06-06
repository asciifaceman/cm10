/*
 * Cycle defines the step sequence data
 * and access handlers around it
 * 
 */
#include <Arduino.h>
#include "ppqn.h"
#include "config.h"
#include "cycle.h"

/*
 * SEQUENCE uint8_t is our 2D array storing a 96 step
 * sequence across 8 divisions of a whole note.
 * Someone mentioned I could put this in the EEPROM but
 * not sure I want to
 *
 * 7: 1/1
 * 6: 1/2
 * 5: 1/3
 * 4: 1/4
 * 3: 1/4-3
 * 2: 1/8
 * 1: 1/16
 * 0: 1/16-3
 */
static uint8_t SEQUENCE[] = {
  255, 0, 1, 0, 1, 0, 3, 0, 9, 0,
  1, 0, 7, 0, 1, 0, 9, 0, 3, 0, 1, 
  0, 1, 0, 31, 0, 1, 0, 1, 0, 3, 0, 
  41, 0, 1, 0, 7, 0, 1, 0, 9, 0, 3, 
  0, 1, 0, 1, 0, 95, 0, 1, 0, 1, 0, 
  3, 0, 9, 0, 1, 0, 7, 0, 1, 0, 41, 
  0, 3, 0, 1, 0, 1, 0, 31, 0, 1, 0,
  1, 0, 3, 0, 9, 0, 1, 0, 7, 0, 1,
  0, 9, 0, 3, 0, 1, 0, 1, 0
};

/*
PORTS:
 * PORTD &= B00000000 = 7, 6, 5, 4, 3, 2, 1, 0
 * PORTB &= B00000000 = x, x, 13, 12, 11, 10, 9, 8
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

// read_step_bytes casts the sequence of bits from this
// step across two bytes to apply to the port registers
StepData read_step_bytes(int step) {
    StepData state = {0, 0};
    uint8_t *data = &SEQUENCE[step];

    // sync out (every step)
    bitSet(state.D, 5);

    /*
     * There may be a more concise way to accomplish this however it appears
     * to operate fast enough for our needs so I am hesitant to touch it
     */

    // 1/1
    if (read_bit(step, 7)){
        bitSet(state.D, 6);
    }

    // 1/2
    if (read_bit(step, 6)){
        bitSet(state.D, 7);
    }

    // 1/3
    if (read_bit(step, 5)){
        bitSet(state.B, 0);
    }

    // 1/4
    if (read_bit(step, 4)){
        bitSet(state.B, 1);
    }

    // 1/4-3
    if (read_bit(step, 3)){
        bitSet(state.B, 2);
    }

    // 1/8
    if (read_bit(step, 2)){
        bitSet(state.B, 3);
    }

    // 1/16
    if (read_bit(step, 1)){
        bitSet(state.B, 4);
    }

    // 1/16-3
    if (read_bit(step, 0)){
        bitSet(state.B, 5);
    }

    return state;
}

/*
 * read_bit returns the state of an individual bit from
 * the `step` uint8_t - counting from the right (little endian)
 */
bool read_bit(int step, uint8_t index) {
  // select the right step from the sequence
  uint8_t *data = &SEQUENCE[step];
   
  return (*data & (1 <<index)) > 0;
}
