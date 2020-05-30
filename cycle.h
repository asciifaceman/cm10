#pragma once

struct StepData {
    byte D;
    byte B;
};

StepData read_step_bytes(int);
bool read_bit (int, uint8_t);


/*
 * Configuration for pin details
 */
/*
enum
{
    WHOLENOTE = 0,
    HALFNOTE,
    TRIPLET,
    QUARTERNOTE,
    QUARTERTRIPLET,
    EIGHTHNOTE,
    SIXTEENTHNOTE,
    SIXTEENTHTRIPLET,
    THIRTYSECONDNOTE
};*/