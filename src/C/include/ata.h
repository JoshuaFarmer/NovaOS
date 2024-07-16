#include "Kernel/io.h"
#include "typedef.h"

#define ATA_PRIMARY_IO 0x1F0
#define ATA_PRIMARY_CTRL 0x3F6
#define ATA_CMD_READ_PIO 0x20
#define ATA_CMD_WRITE_PIO 0x30
#define ATA_REG_HDDEVSEL 0x06
#define STATUS_RDY 0x40
#define IRQ_MASK_ATA_PRIMARY 0x40 // Mask for IRQ 14 (Primary ATA)
#define IRQ_MASK_ATA_SECONDARY 0x80 // Mask for IRQ 15 (Secondary ATA)
#define ATA_PRIMARY_CONTROL_BASE 0x3F6

extern error_t printf(const char*, ...);

void disable_ata_irq();
void ata_wait_bsy();
void ata_wait_drq();
void ata_select_drive(uint8_t drive);
void identify_ata(uint8_t drive);
void ata_read_sector(uint32_t lba, uint8_t *buffer);
void ata_write_sector(uint32_t lba, const byte_t* buffer);
uint8_t ata_disk_status();
uint8_t init_ata();
void ata_read(byte_t* buff, uint32_t lba, uint32_t count);
void ata_write(const byte_t* buff, uint32_t lba, uint32_t count);