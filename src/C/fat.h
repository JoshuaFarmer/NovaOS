#pragma once

#include "string.h"
#include "ata.h"

#define FAT32_SIGNATURE 0xAA55

// Define necessary data structures
#pragma pack(push, 1)

typedef struct {
    uint8_t  jump[3];
    uint8_t  oem[8];
    uint16_t bytes_per_sector;
    uint8_t  sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t  fat_count;
    uint16_t root_entry_count;
    uint16_t total_sectors_16;
    uint8_t  media_descriptor;
    uint16_t sectors_per_fat_16;
    uint16_t sectors_per_track;
    uint16_t head_count;
    uint32_t hidden_sectors;
    uint32_t total_sectors_32;
    uint32_t sectors_per_fat_32;
    uint16_t flags;
    uint16_t version;
    uint32_t root_cluster;
    uint16_t fsinfo_sector;
    uint16_t backup_boot_sector;
    uint8_t  reserved[12];
    uint8_t  drive_number;
    uint8_t  reserved_nt;
    uint8_t  boot_signature;
    uint32_t volume_id;
    uint8_t  volume_label[11];
    uint8_t  fs_type[8];
    uint8_t  boot_code[420];
    uint16_t boot_sector_signature;
} fat32_boot_sector_t;

typedef struct {
    uint8_t  name[11];
    uint8_t  attr;
    uint8_t  nt_reserved;
    uint8_t  time_tenth;
    uint16_t create_time;
    uint16_t create_date;
    uint16_t access_date;
    uint16_t cluster_high;
    uint16_t write_time;
    uint16_t write_date;
    uint16_t cluster_low;
    uint32_t size;
} fat32_dir_entry_t;
#pragma pack(pop)

// Function to read the boot sector
void read_boot_sector(uint8_t drive, fat32_boot_sector_t *bs) {
    LBA28_read_sector(drive, 0, 1, (uint16_t*)bs);
}

// Function to read a FAT entry
uint32_t read_fat_entry(uint8_t drive, fat32_boot_sector_t *bs, uint32_t cluster) {
    uint32_t fat_offset = cluster * 4;
    uint32_t fat_sector = bs->reserved_sectors + (fat_offset / bs->bytes_per_sector);
    uint32_t ent_offset = fat_offset % bs->bytes_per_sector;

    uint16_t sector_data[256];
    LBA28_read_sector(drive, fat_sector, 1, sector_data);

    return (*(uint32_t*)((uint8_t*)sector_data + ent_offset)) & 0x0FFFFFFF;
}

// Function to read a directory entry
int read_dir_entry(uint8_t drive, fat32_boot_sector_t *bs, const char *filename, fat32_dir_entry_t *dir) {
    uint32_t root_dir_sector = bs->reserved_sectors + bs->fat_count * bs->sectors_per_fat_32;
    uint32_t dir_entries = bs->bytes_per_sector / sizeof(fat32_dir_entry_t);

    fat32_dir_entry_t entries[dir_entries];

    for (uint32_t i = 0; i < bs->sectors_per_cluster; i++) {
        LBA28_read_sector(drive, root_dir_sector + i, 1, (uint16_t*)entries);
        for (uint32_t j = 0; j < dir_entries; j++) {
            if (strncmp((const char*)entries[j].name, filename, 11) == 0) {
                *dir = entries[j];
                return 1;
            }
        }
    }
    return 0;
}

// Function to read file data
void read_file_data(uint8_t drive, fat32_boot_sector_t *bs, fat32_dir_entry_t *dir, uint8_t *buffer) {
    uint32_t cluster = (dir->cluster_high << 16) | dir->cluster_low;
    uint32_t bytes_per_cluster = bs->bytes_per_sector * bs->sectors_per_cluster;
    uint32_t bytes_remaining = dir->size;

    while (bytes_remaining > 0) {
        uint32_t sector = bs->reserved_sectors + bs->fat_count * bs->sectors_per_fat_32 + (cluster - 2) * bs->sectors_per_cluster;
        uint32_t sectors_to_read = bs->sectors_per_cluster;
        if (sectors_to_read * bs->bytes_per_sector > bytes_remaining) {
            sectors_to_read = (bytes_remaining + bs->bytes_per_sector - 1) / bs->bytes_per_sector;
        }
        LBA28_read_sector(drive, sector, sectors_to_read, (uint16_t*)buffer);
        uint32_t bytes_to_copy = bytes_remaining < bytes_per_cluster ? bytes_remaining : bytes_per_cluster;
        buffer += bytes_to_copy;
        bytes_remaining -= bytes_to_copy;
        cluster = read_fat_entry(drive, bs, cluster);
    }
}

// Function to read a file by name
void read_file(uint8_t drive, const char *filename, uint8_t *buffer) {
    fat32_boot_sector_t bs;
    fat32_dir_entry_t dir;

    read_boot_sector(drive, &bs);

    if (read_dir_entry(drive, &bs, filename, &dir)) {
        read_file_data(drive, &bs, &dir, buffer);
    } else {
        puts_coloured("File not found\n", VGA_COLOR_LIGHT_RED);
    }
}