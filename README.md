# CM10 - Modular Synth Clock
An arduino project to power the CM10 clock core.
Originally based on [https://github.com/JerrySievert/EurorackClock](https://github.com/JerrySievert/EurorackClock)

This project is for a Modular Synth Clock, providing sync in and out MIDI PPQN (24 pulses per quarter note) and multiple subdivisions such as triplets, whole notes, quarter notes, etc.

# Hardware & Schematic
TODO

## Inputs

## Outputs

# Software

## OLED Screen
This project is set up to use an I2C screen.

Displays if SYNC IN, SYNC OUT, and BPM

## Configuration
Configuration is provided via [config.h](config.h)

* DEBUG - enables serial debug messaging
* NOIO - `true` disables analog read and uses `NOIO_BPM` with `NOIO_DURATION`
* NOIO_BPM - [default: `120`] the static BPM to use if NOIO
* NOIO_DURATION - [default: `50`] % duration if NOIO
* MIN_BPM [default: `60`] - the minimum bpm
* MAX_BPM [default: `220`] - the maximum bpm
* BIAS [default: `25`] - minimum milliseconds for duration
* HAS_SCREEN [default: `false`] - whether a screen is attached