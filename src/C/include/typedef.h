#pragma once

#define GVGA_WIDTH 320
#define GVGA_HEIGHT 200
#define CLOSE_BUTTON_SIZE 8
#define CLOSE_BUTTON_XMARGIN 3
#define CONTROL_HEIGHT 15
#define WIN_MARGIN 5

#define MAX_WINDOW_COUNT 25
#define MAX_ELEMENT_COUNT 25

#define MIN_WIN_W 32
#define MIN_WIN_H 24

#define DESKTOP_C 230

#define NULL (void*)0

#define true 1
#define false 0

#define update_screen(src) memcpy((void*)0xA0000, src, GVGA_WIDTH*GVGA_HEIGHT);
#define putp(buff, x, y, c) buff[(GVGA_WIDTH*(y)) + (x)] = c

#define CURS_W 10
#define CURS_H 12
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

#define vgabuff 0xA0000
#define videobuff 0xB8000
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60
#define STATUS_BSY	  0x80
#define STATUS_RDY	  0x40
#define STATUS_DRQ	  0x08
#define STATUS_DF		0x20
#define STATUS_ERR	  0x01
#define read_sector(drive, sector, buffer) LBA28_read_sector(drive, sector, 1, buffer)
#define write_sector(drive, sector, data) LBA28_write_sector(drive, sector, 1, data)
#define PIT_CHANNEL0_PORT 0x40
#define PIT_COMMAND_PORT 0x43
#define SECTOR_SIZE 512
#define ROOT_DIR_SECTOR 2
#define RESERVED_SECTORS 32
#define FAT_SECTORS 32
#define cli() asm("cli");
#define sti() asm("sti");

typedef unsigned long long  uint64_t;
typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;

typedef long long int64_t;
typedef int   int32_t;
typedef short int16_t;
typedef char  int8_t;

typedef uint8_t  byte_t;
typedef uint16_t word_t;
typedef uint32_t dword_t;
typedef uint64_t qword_t;

typedef int8_t  sbyte_t;
typedef int16_t sword_t;
typedef int32_t sdword_t;
typedef int64_t sqword_t;

typedef int32_t wchar_t;

typedef uint64_t size_t;
typedef uint8_t bool_t;

typedef uint32_t error_t;
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;
typedef WORD WCHAR;
typedef uint32_t UINT;
typedef uint32_t LBA_t;
typedef uint32_t FSIZE_t;
