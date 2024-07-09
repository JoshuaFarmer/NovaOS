#pragma once
#define FF_FS_READONLY 0

#include <stdarg.h>
#include "typedef.h"

uint8_t system_user[64];
bool_t running = true;

#define setcolor(color) { colour = color }
void system(const uint16_t* sys);

#include "Kernel/math.h"
#include "Kernel/io.h"
#include "Kernel/terminal.h"
#include "Kernel/sound.h"
#include "Kernel/shell.h"

//#include "ff.h" // Include FatFs header file

#include "Kernel/string.h"
#include "Kernel/malloc.h"
//#include "Kernel/ata.h"
//#include "Kernel/diskio.h"
//#include "ff.h"
//#include "ffconf.h"

//#include "emu.h"
#include "Kernel/video.h"

// Setup VGA buffer. Initialize the VGA frame buffer. (I think - x4exr)