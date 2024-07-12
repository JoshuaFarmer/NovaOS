#pragma once
#include "typedef.h"
#include "Kernel/io.h"

void wait_ms(unsigned int milliseconds);
void playSound(uint32_t nFrequence);
void shutup();
void beep(uint32_t pitch, uint32_t ms);
