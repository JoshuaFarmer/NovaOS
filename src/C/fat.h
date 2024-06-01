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
	int total_sectors = (fat_boot->total_sectors_16 == 0)? fat_boot->total_sectors_32 : fat_boot->total_sectors_16;
	return total_sectors;
}

uint32_t get_fat_size(fat_BS_t* fat_boot) {
	int fat_size = fat_boot->table_size_16;
	return fat_size;
}

uint32_t get_root_dir_sectors(fat_BS_t* fat_boot) {
	int root_dir_sectors = ((fat_boot->root_entry_count * 32) + (fat_boot->bytes_per_sector - 1)) / fat_boot->bytes_per_sector;
	return root_dir_sectors;
}

uint32_t get_first_data_sector(fat_BS_t* fat_boot) {
	int first_data_sector = fat_boot->reserved_sector_count + (fat_boot->table_count * get_fat_size(fat_boot) + get_root_dir_sectors(fat_boot));
	return first_data_sector;
}

uint32_t get_first_fat(fat_BS_t* fat_boot) {
	int first_fat_sector = fat_boot->reserved_sector_count;
	return first_fat_sector;
}

uint32_t get_total_data_sectors(fat_BS_t* fat_boot) {
	int data_sectors = get_total_sectors(fat_boot) - (fat_boot->reserved_sector_count + (fat_boot->table_count * get_fat_size(fat_boot)) + get_root_dir_sectors(fat_boot));
	return data_sectors;
}

uint32_t get_total_clusters(fat_BS_t* fat_boot) {
	int total_clusters = get_total_data_sectors(fat_boot) / fat_boot->sectors_per_cluster;
	return total_clusters;
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
	ExFAT_c,
} FatType_t;

FatType_t get_fat_type(fat_BS_t* fat_boot) {
	if (fat_boot->bytes_per_sector == 0) { return ExFAT_c; }

	const uint32_t total_clusters = get_total_clusters(fat_boot);
	if (total_clusters < 4085) { return FAT12_c; }
	if (total_clusters < 65525) { return FAT16_c; }
	else { return FAT32_c; }
}
