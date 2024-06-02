#pragma once
#include "string.h"
#include "ata.h"

#define FAT32_SIGNATURE 0xAA55

typedef struct fat_extBS_32
{
	//extended fat32 stuff
	unsigned int		table_size_32;
	unsigned short		extended_flags;
	unsigned short		fat_version;
	unsigned int		root_cluster;
	unsigned short		fat_info;
	unsigned short		backup_BS_sector;
	unsigned char 		reserved_0[12];
	unsigned char		drive_number;
	unsigned char 		reserved_1;
	unsigned char		boot_signature;
	unsigned int 		volume_id;
	unsigned char		volume_label[11];
	unsigned char		fat_type_label[8];
 
}__attribute__((packed)) fat_extBS_32_t;
 
typedef struct fat_extBS_16
{
	//extended fat12 and fat16 stuff
	unsigned char		bios_drive_num;
	unsigned char		reserved1;
	unsigned char		boot_signature;
	unsigned int		volume_id;
	unsigned char		volume_label[11];
	unsigned char		fat_type_label[8];
 
}__attribute__((packed)) fat_extBS_16_t;
 
typedef struct fat_BS
{
	unsigned char 		bootjmp[3];
	unsigned char 		oem_name[8];
	unsigned short 		bytes_per_sector;
	unsigned char		sectors_per_cluster;
	unsigned short		reserved_sector_count;
	unsigned char		table_count;
	unsigned short		root_entry_count;
	unsigned short		total_sectors_16;
	unsigned char		media_type;
	unsigned short		table_size_16;
	unsigned short		sectors_per_track;
	unsigned short		head_side_count;
	unsigned int 		hidden_sector_count;
	unsigned int 		total_sectors_32;
 
	//this will be cast to it's specific type once the driver actually knows what type of FAT this is.
	unsigned char		extended_section[54];
 
}__attribute__((packed)) fat_BS_t;

uint32_t get_total_sectors(fat_BS_t* fat_boot) {
    return (fat_boot->total_sectors_16 == 0) ? fat_boot->total_sectors_32 : fat_boot->total_sectors_16;
}

uint32_t get_fat_size(fat_BS_t* fat_boot) {
    return fat_boot->table_size_16;
}

uint32_t get_root_dir_sectors(fat_BS_t* fat_boot) {
    return ((fat_boot->root_entry_count * 32) + (fat_boot->bytes_per_sector - 1)) / fat_boot->bytes_per_sector;
}

uint32_t get_first_data_sector(fat_BS_t* fat_boot) {
    return fat_boot->reserved_sector_count + (fat_boot->table_count * get_fat_size(fat_boot)) + get_root_dir_sectors(fat_boot);
}

uint32_t get_total_data_sectors(fat_BS_t* fat_boot) {
    return get_total_sectors(fat_boot) - (fat_boot->reserved_sector_count + (fat_boot->table_count * get_fat_size(fat_boot)) + get_root_dir_sectors(fat_boot));
}

uint32_t get_total_clusters(fat_BS_t* fat_boot) {
    return ((fat_boot->total_sectors_16 == 0) ? fat_boot->total_sectors_32 : fat_boot->total_sectors_16) / fat_boot->sectors_per_cluster;
}

uint32_t get_first_root_dir_sector(fat_BS_t* fat_boot) {
	int first_root_dir_sector = get_first_data_sector(fat_boot) - get_root_dir_sectors(fat_boot);
	return first_root_dir_sector;
}

uint32_t get_first_root_dir_cluster32(fat_extBS_32_t* fat_boot) {
	int root_cluster_32 = fat_boot->root_cluster;
	return root_cluster_32;
}

uint32_t get_first_sector_of_cluster(fat_BS_t* fat_boot, uint32_t cluster) {
	int first_sector_of_cluster = ((cluster - 2) * fat_boot->sectors_per_cluster) + get_first_data_sector(fat_boot);
	return first_sector_of_cluster;
}

typedef struct fat_file_attr {
	int read_only:1;
	int hidden:1;
	int system:1;
	int vol:1;
	int dir:1;
	int archive:1;
	int device:1;
	int unused:1;
}__attribute__ ((packed)) fat_file_attr_t;

/* file date */
typedef struct fat_file_date {
	uint32_t day:5;
	uint32_t month:4;
	uint32_t year:7;
}__attribute__ ((packed)) fat_file_date_t;

/* file time */
typedef struct fat_file_time {
	uint32_t sec:5;
	uint32_t min:6;
	uint32_t hour:5;
}__attribute__ ((packed)) fat_file_time_t;

typedef struct fat_file {
	uint8_t name[8];
	uint8_t ext[3];
	uint8_t attr;
	uint8_t winnt_flags;
	uint8_t creattimesecs;
	uint16_t creattime;
	uint16_t creatdate;
	uint16_t lastacces;
	uint16_t h_firstclus;
	fat_file_time_t lm_time;
	fat_file_date_t lm_date;
	uint16_t l_firstclus;
	uint32_t size;
}__attribute__((packed)) fat_file_t;

typedef struct fat_file_long {
	uint8_t order;
	uint16_t first_5[5];
	uint8_t attr;
	uint8_t long_entry_type;
	uint8_t checksum;
	uint8_t next_6[12];
	uint16_t zeros;
	uint8_t final_2[4];
}__attribute__((packed)) fat_file_long_t;

typedef enum {
	FAT12_c,
	FAT16_c,
	FAT32_c,
} FatType_t;

uint32_t get_first_fat(fat_BS_t* fat_boot) {
	int first_fat_sector = fat_boot->reserved_sector_count;
	return first_fat_sector;
}

FatType_t get_fat_type(fat_BS_t* fat_boot) {
    uint32_t total_clusters = get_total_clusters(fat_boot);
    if (total_clusters < 4085) { return FAT12_c; }
    if (total_clusters < 65525) { return FAT16_c; }
    return FAT32_c;
}

void read_boot_sector(uint8_t drive, fat_BS_t *bs) {
    LBA28_read_sector(drive, 0, 1, (uint16_t*)bs);
    printf("%T10.bytes_per_sector: %T14.%d\n", bs->bytes_per_sector);
    printf("%T10.sectors_per_cluster: %T14.%d\n", bs->sectors_per_cluster);
    printf("%T10.reserved_sector_count: %T14.%d\n", bs->reserved_sector_count);
    printf("%T10.table_count: %T14.%d\n", bs->table_count);
    printf("%T10.total_sectors_16: %T14.%d\n", bs->total_sectors_16);
    printf("%T10.total_sectors_32: %T14.%d\n", bs->total_sectors_32);
    printf("%T10.table_size_16: %T14.%d\n", bs->table_size_16);
}


// Function to compare file names (8.3 format)
int compare_filenames(const char* filename1, const char* filename2) {
	return strncmp(filename1, filename2, 11) == 0;
}

void read_file(uint8_t drive, const char* filename, uint8_t* buffer, uint32_t buffer_size) {
	asm("cli");

    fat_BS_t bs;
    read_boot_sector(drive, &bs);

    FatType_t fat_type = get_fat_type(&bs);
    if (fat_type != FAT32_c) {
        // Only FAT32 is supported by this function
        printf("Unsupported FAT type\n");
        return;
    }

    // Get the first sector of the root directory
    fat_extBS_32_t* fat_ext_bs = (fat_extBS_32_t*)(&bs.extended_section);
    uint32_t root_cluster = get_first_root_dir_cluster32(fat_ext_bs);
    uint32_t first_sector = get_first_sector_of_cluster(&bs, root_cluster);

    // Loop through root directory entries to find the file
    fat_file_t file_entry;
    uint32_t sector = first_sector;
    uint32_t entry_count = 0;
    int file_found = 0;

    printf("Searching for file in root directory...\n");

    while (!file_found && sector < (first_sector + bs.sectors_per_cluster)) {
        LBA28_read_sector(drive, sector, 1, (uint16_t*)&file_entry);

        // Loop through each file entry in the sector
        for (uint32_t i = 0; i < bs.bytes_per_sector / sizeof(fat_file_t); i++) {
            if (file_entry.name[0] == 0x00) {
                // No more entries in this directory
                printf("End of directory entries\n");
                break;
            }

            if ((file_entry.attr & 0x0F) == 0x0F) {
                // Long file name entry, skip it
                continue;
            }

            if (compare_filenames((char*)file_entry.name, filename)) {
                // File found
                file_found = 1;
                printf("File found: %s\n", filename);
                break;
            }

            entry_count++;
        }

        sector++;
    }

    if (!file_found) {
        // File not found
        printf("File not found: %s\n", filename);
        return;
    }

    // Read the file
    uint32_t cluster = ((uint32_t)file_entry.h_firstclus << 16) | file_entry.l_firstclus;
    uint32_t bytes_read = 0;

    printf("Reading file data...\n");

    while (cluster >= 2 && cluster < 0x0FFFFFF8 && bytes_read < buffer_size) {
        sector = get_first_sector_of_cluster(&bs, cluster);
        uint32_t sectors_to_read = bs.sectors_per_cluster;

        for (uint32_t i = 0; i < sectors_to_read && bytes_read < buffer_size; i++) {
            LBA28_read_sector(drive, sector + i, 1, (uint16_t*)(buffer + bytes_read));
            bytes_read += bs.bytes_per_sector;
        }

        // Get the next cluster
        uint32_t fat_sector = get_first_fat(&bs) + (cluster * 4 / bs.bytes_per_sector);
        uint32_t fat_offset = (cluster * 4) % bs.bytes_per_sector;
        uint32_t next_cluster;

        LBA28_read_sector(drive, fat_sector, 1, (uint16_t*)&next_cluster);
        cluster = next_cluster >> fat_offset;
    }

    printf("File read complete, bytes read: %u\n", bytes_read);
}