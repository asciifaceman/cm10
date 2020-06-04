# CM10 Clock - Modular Synth Core Clock

# About me
This is my first C++ project, and my first serious Ardudino project. If I'm doing anything really stupid, please open a tracker issue - I want to know.

# Clock Design
This clock was designed with MIDI Sync IO in mind, thus the core of the clock tracks in 24 pulses per quarter note (ppqn), resulting in a 96 step sequence for each whole note.

# Hardware
Currently this project is locked into Arduino Nano due to writing directly to port registers. I intend on adding more functionality to support other boards, however I don't own any and cannot test yet.

### Ports
* D2 (interrupt) - Clock halt/Reset (no reboot)
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

Codewise, these are represented across two bytes:

```
 PORTD = B00000000 = 7, 6, 5, 4, 3, 2, 1, 0
 PORTB = B00000000 = x, x, 13, 12, 11, 10, 9, 8
```

### INPUTS
* Course BPM Tune
* Pulse Width (duration)
* SYNC IN
* HALT/RESET

### OUTPUTS
* SYNC OUT
* Whole Note
* Half note
* Triplet
* Quarter Note
* Quarter Triplet
* Eigth Note
* Sixteenth Note
* Sixteenth Triplet

### OLED Screen
This project is set up to support a 128x64 I2C screen via the Adafruit library.
It only displays when the clock is halted by the button input on D2

# Configuration
Configuration is provided dby [config.h](config.h)

* DEBUG
    * Enables serial console and streams log
* NOIO
    * Enable if no potentiometers wired to use a static preset BPM
* NOIO_BPM
    * NOIO static BPM
* NOIO_DURATION
    * NOIO static pulse duration
* MIN_BPM
    * Lowest BPM the clock can achieve
* MAX_BPM
    * Highest BPM the clock can achieve
* DURATION_BIAS
    * Pulse duration bias, the shortest time the pulse will last
* HAS_SCREEN
    * Whether a screen is attached/expected
* SCREEN_ADDRR
    * The address of the screen

# Possible Ideas
* Clock rotation mode
    * A silly possible idea that will cause the clock outputs to start "rotating", shifting along the ports sequentially in a loop with a control for rotation speed
