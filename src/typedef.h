#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef struct block {
	size_t size;
	struct block* next;
	int free;
} block_t;

typedef struct {
	uint8_t filename[8];
	uint8_t ext[3];
	uint8_t attributes;
	uint8_t reserved;
	uint8_t creation_time_tenths;
	uint16_t creation_time;
	uint16_t creation_date;
	uint16_t last_access_date;
	uint16_t first_cluster_high;
	uint16_t last_modification_time;
	uint16_t last_modification_date;
	uint16_t first_cluster_low;
	uint32_t file_size;
} DirectoryEntry;

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

typedef struct{
	char drivetype;
	short sectors;
	short cylinders;
	char heads;
	char drives;
} disk_info;

typedef struct{
	char bl;
	char ch;
	char cl;
	char dh;
	char dl;
} raw_disk_info;

typedef struct {
	uint8_t jump[3];
	uint8_t oem_name[8];
	uint16_t bytes_per_sector;
	uint8_t sectors_per_cluster;
	uint16_t reserved_sectors;
	uint8_t fat_count;
	uint16_t root_dir_entries;
	uint16_t total_sectors_short;
	uint8_t media_descriptor;
	uint16_t sectors_per_fat;
	uint16_t sectors_per_track;
	uint16_t heads;
	uint32_t hidden_sectors;
	uint32_t total_sectors_long;
	uint32_t sectors_per_fat32;
	uint16_t ext_flags;
	uint16_t fs_version;
	uint32_t root_cluster;
	uint16_t fs_info_sector;
	uint16_t backup_boot_sector;
	uint8_t reserved[12];
	uint8_t drive_number;
	uint8_t reserved1;
	uint8_t boot_signature;
	uint32_t volume_id;
	uint8_t volume_label[11];
	uint8_t fs_type[8];
	uint8_t boot_code[420];
	uint16_t boot_sector_signature;
} FAT32BootSector;

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