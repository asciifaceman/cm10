#pragma once

struct StepData {
    byte D;
    byte B;
};

StepData read_step_bytes(int);
bool read_bit (int, uint8_t);
