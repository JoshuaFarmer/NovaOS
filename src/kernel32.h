#pragma once
#define videobuff 0xB8000
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

void putc(char c);
void putcc(char c, uint8_t col);

#pragma once
// Define a structure to represent memory blocks
typedef struct block {
	size_t size;		  // Size of the memory block
	struct block* next;   // Pointer to the next block in the list
	int free;			 // Flag to indicate whether the block is free (1) or allocated (0)
} block_t;

// Pointer to the start of the heap
void* heap_start = NULL;

void* malloc(size_t size);

// Function to initialize the heap
void init_heap(size_t initial_size) {
	heap_start = malloc(initial_size);
	if (heap_start != NULL) {
		((block_t*)heap_start)->size = initial_size - sizeof(block_t);
		((block_t*)heap_start)->next = NULL;
		((block_t*)heap_start)->free = 1;
	}
}

// Function to allocate memory
void* malloc(size_t size) {
	block_t* curr = (block_t*)heap_start;
	block_t* best_fit = NULL;

	// Find the first fit block that can accommodate the requested size
	while (curr != NULL) {
		if (curr->free && curr->size >= size && (best_fit == NULL || curr->size < best_fit->size)) {
			best_fit = curr;
		}
		curr = curr->next;
	}

	// If no suitable block is found, return NULL
	if (best_fit == NULL) {
		return NULL;
	}

	// If the block is larger than needed, split it
	if (best_fit->size > size + sizeof(block_t)) {
		block_t* new_block = (block_t*)((char*)best_fit + sizeof(block_t) + size);
		new_block->size = best_fit->size - size - sizeof(block_t);
		new_block->next = best_fit->next;
		new_block->free = 1;
		best_fit->size = size;
		best_fit->next = new_block;
	}

	// Mark the block as allocated and return a pointer to the user
	best_fit->free = 0;
	return (void*)(best_fit + 1);
}

// Function to free memory
void free(void* ptr) {
	if (ptr == NULL) {
		return;
	}

	// Get a pointer to the block header
	block_t* block = (block_t*)ptr - 1;

	// Mark the block as free
	block->free = 1;

	// Coalesce adjacent free blocks
	block_t* curr = (block_t*)heap_start;
	while (curr != NULL && curr->next != NULL) {
		if (curr->free && curr->next->free) {
			curr->size += sizeof(block_t) + curr->next->size;
			curr->next = curr->next->next;
		}
		curr = curr->next;
	}
}

size_t txty;
size_t txtx;

/* Hardware text mode color constants. */
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


uint8_t colour;
uint16_t* txtbuff;
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

static inline unsigned char inb(unsigned short port) {
	unsigned char ret;
	asm("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

static inline void outb(unsigned short port, unsigned char value) {
	asm("outb %0, %1" : : "a"(value), "Nd"(port));
}

// Write a word (16 bits) to a port
void outw(uint16_t port, uint16_t value) {
	asm volatile ("outw %0, %1" :: "a"(value), "Nd"(port));
}

// Read a word (16 bits) from a port
uint16_t inw(uint16_t port) {
	uint16_t value;
	asm volatile ("inw %1, %0" : "=a"(value) : "Nd"(port));
	return value;
}

void outl(uint16_t port, uint32_t value){
	asm volatile ("outl %%eax, %%dx" :: "d" (port), "a" (value));
}

uint32_t inl(uint16_t port){
	uint32_t ret;
	asm volatile ("inl %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
} 

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

int strcmp(const char *s1, const char *s2) {
	while (*s1 && (*s1 == *s2)) {
		s1++;
		s2++;
	}
	return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

void *memset(void *ptr, int value, size_t num) {
	unsigned char *p = ptr;
	unsigned char v = value;
	for (size_t i = 0; i < num; i++) {
		p[i] = v;
	}
	return ptr;
}


// 27 == esc
// 0xf1 - 0xfc are function keys
// 0xfe is numlock
// 0xfd is alt

unsigned char keyboard_map[256] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 'C', 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '#', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', 0xFD, '*', 'A', ' ',
	'C', 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFE, 'n', '7',
	'8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\n',
	'A', 'B', 0xFB, 0xFC
};

unsigned char keyboard_map_shifted[256] = {
	0, 27, '!', '"', '$', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 'C', 'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '@', '`', 0, '#', 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', ',', '.', '?', 0xFD, '*', 'A', ' ',
	'C', 0XF1, 0XF2, 0XF3, 0XF4, 0XF5, 0XF6, 0XF7, 0XF8, 0XF9, 0XFA, 0xFE, 0xFE, '7',
	'8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\n',
	'A', 'B', 0XFB, 0XFC
};

char getch() {
	char status;
	do {
		status = inb(KEYBOARD_STATUS_PORT); // Read keyboard status port
	} while ((status & 0x01) == 0); // Wait until there is data to read

	unsigned char scancode = inb(KEYBOARD_DATA_PORT); // Read scan code

	static int shift_pressed = 0;

	if (scancode == 0x2A) { // Scan code for Left Shift pressed
		shift_pressed = 1;
		return 0; // Ignore Shift key press itself
	} else if (scancode == 0xAA) { // Scan code for Left Shift released
		shift_pressed = 0;
		return 0; // Ignore Shift key release itself
	}

	if (scancode & 0x80) {
		return 0;
	} else {
		// Check if Shift key is pressed and adjust keyboard map index accordingly
		if (shift_pressed) {
			return keyboard_map_shifted[scancode];
		} else {
			return keyboard_map[scancode];
		}
	}
}

void gets(char *buffer, int buffer_size) {
	int index = 0;
	char c;

	while (index < buffer_size - 1) { // Ensure space for null-terminator
		c = getch();

		if (c == '\n') { // Check for Enter key
			buffer[index] = '\0'; // Null-terminate the string
			putc('\n');
			return;
		} else if (c == '\b') { // Check for backspace
			if (index > 0) { // Ensure there are characters to delete
				index--;
				putc('\b'); // Move cursor back
				putc(' '); // Erase character
				putc('\b'); // Move cursor back again
			}
		} else if (c >= ' ' && c <= '~') { // Check if printable ASCII character
			buffer[index++] = c; // Store printable character in buffer
			putc(c); // Echo the character to the screen
		}
	}

	// If the loop exits, the buffer is full
	buffer[index] = '\0'; // Null-terminate the string
	putc('\n');
}

void update_cursor(int x, int y) {
	uint16_t pos = y * VGA_WIDTH + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void putch(char c, uint8_t colour, size_t txtx, size_t txty) {
	if (txty < 0) txty = 0;
	if (txtx < 0) txtx = 0;
	
	if (c == '\b') return;
	if (c == 0) return;
	if (c == '\n') return;

	// normal
	((char*)videobuff)[(txty*80*2)+(txtx*2)] = c;
	((char*)videobuff)[(txty*80*2)+(txtx*2)+1] = colour;
	outb(0xe9, c);
}

void putc(char c) {
	if (txty < 0) txty = 0;
	if (txtx < 0) txtx = 0;
	// backspace
	if (c == '\b') {--txtx;
		if (txtx < 0) {
			txtx = 0; --txty;
			while ( ((char*)videobuff)[(txty*80*2) + (txtx*2)] != '\0') {txtx++;}
		}
		((char*)videobuff)[(txty*80*2)+(txtx*2)] = '\0';
		((char*)videobuff)[(txty*80*2)+(txtx*2)+1] = colour;
		outb(0xe9, c);
		update_cursor(txtx, txty);
		return;
	}

	// NULL
	if (c == 0) return;
	if (c == '\n') {txtx = 0; txty++; update_cursor(txtx, txty); return;}

	// normal
	((char*)videobuff)[(txty*80*2)+(txtx*2)] = c;
	((char*)videobuff)[(txty*80*2)+(txtx*2)+1] = colour;
	if (txtx <= 80) {
		txtx++;
	} else {
		txtx = 0; txty++;
	}
	outb(0xe9, c);
	update_cursor(txtx, txty);
}

uint8_t hchar(uint8_t c) {
	c&=15;
	if (c <= 9) {
		return c + '0';
	} else if (c >= 0xA && c <= 0xF) {
		return c + ('A'-10);
	}
	return 0;
}

void puth(uint8_t a) {
	char c = hchar(a>>4);
	putc(c);
	c = hchar(a&15);
	putc(c);
}

void putcc(char c, uint8_t col)  {
	uint8_t tmp = colour;
	colour = col;
	putc(c);
	colour = tmp;
}

void putss(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		putc(data[i]);
}

void putsc(const char* data, uint8_t col) {
	uint8_t tmp = colour;
	colour = col;
	putss(data, strlen(data));
	colour = tmp;
}

void puts(const char* data) {
	putss(data, strlen(data));
}

void* memcpy(void *dest, const void *src, size_t len) {
	char *d = (char*)dest;
	const char *s = (char*)src;
	while (len--)
		*d++ = *s++;
	return dest;
}

// Simple (P)Ata HDD (Hard Disk Drive) Polling Driver using PIO Mode (instead of the better DMA)
// Inspirations and Sources: (https://wiki.osdev.org/ATA_PIO_Mode)

typedef struct{
	char drivetype;
	short sectors;
	short cylinders;
	char heads;
	char drives;
}
disk_info;

typedef struct{
	char bl;
	char ch;
	char cl;
	char dh;
	char dl;
}
raw_disk_info;

void identify_ata(uint8_t drive);
// sector means the amount of sectors you wanna read
uint16_t* LBA28_read_sector(uint8_t drive, uint32_t LBA, uint32_t sector, uint16_t *addr);
void LBA28_write_sector(uint8_t drive, uint32_t LBA, uint32_t sector, uint16_t *buffer);

void decode_raw_disk_info(raw_disk_info dinfo, disk_info * result);
raw_disk_info retrieve_disk_info();

#define STATUS_BSY	  0x80
#define STATUS_RDY	  0x40
#define STATUS_DRQ	  0x08
#define STATUS_DF		0x20
#define STATUS_ERR	  0x01

void identify_ata(uint8_t drive){
	// 0xA0 for Master
	// 0xB0 for Slave

	outb(0x1F6, drive);
	outb(0x1F2, 0);
	outb(0x1F3, 0);
	outb(0x1F4, 0);
	outb(0x1F5, 0);
	outb(0x1F7, 0xEC);
	uint8_t tmp = inb(0x1F7);
	if(tmp & STATUS_RDY){
		switch(drive){
			case 0xA0:
				putsc("Master Drive is the current active Drive\n", VGA_COLOR_LIGHT_GREEN);
				break;
			case 0xB0:
				putsc("Slave Drive is the current active Drive\n", VGA_COLOR_LIGHT_GREEN);
				break;
		}
	}
	else
	{
		switch(drive){
			case 0xA0:
				putsc("Master Drive is NOT the current active Drive \n", VGA_COLOR_LIGHT_RED);
				break;
			case 0xB0:
				putsc("Slave Drive is NOT the current active Drive \n", VGA_COLOR_LIGHT_RED);
				break;
			}
	}
}

// TODO implement something on the lines of "took too long to respond"
void wait_BSY(){
	while(inb(0x1F7) & STATUS_BSY);
}

void wait_DRQ(){
	while(!(inb(0x1F7) & STATUS_RDY));
}

uint16_t* LBA28_read_sector(uint8_t drive, uint32_t LBA, uint32_t sector, uint16_t *addr){
	LBA &= 0x0FFFFFFF;
	
	wait_BSY();
	outb(0x1F6, drive | ((LBA >> 24) & 0xF));
	outb(0x1F1, 0x00);
	outb(0x1F2, sector);
	outb(0x1F3, (uint8_t) LBA);
	outb(0x1F4, (uint8_t)(LBA >> 8));
	outb(0x1F5, (uint8_t)(LBA >> 16)); 
	outb(0x1F7, 0x20); // 0x20 = 'Read' Command

	
	uint16_t *tmp = addr;
	
	for (uint32_t j = 0; j < sector; j ++){
		wait_BSY();
		wait_DRQ();
		for(uint32_t i = 0; i < 256; i++){
			tmp[i] = inw(0x1F0);
		}

		tmp += 256;
	}
	return addr;
}


// WARNING this only writes the lowest 8 bits of what's in the buffer, and leaves gaps. Idk if this can be done another way, in such case please look into it.
void LBA28_write_sector(uint8_t drive, uint32_t LBA, uint32_t sector, uint16_t *buffer){
	LBA &= 0x0FFFFFFF;
	
	wait_BSY();
	outb(0x1F6, drive | ((LBA >> 24) & 0xF));		// send drive and bits 24 - 27 of LBA
	outb(0x1F1, 0x00);								// ?
	outb(0x1F2, sector);							// send number of sectors
	outb(0x1F3, (uint8_t) LBA);						// send bits 0-7 of LBA
	outb(0x1F4, (uint8_t) (LBA >> 8));				// 8-15
	outb(0x1F5, (uint8_t) (LBA >> 16)); 			// 16-23
	outb(0x1F7,0x30); 								// 0x30 = 'Write' Command

	uint32_t *tmp = (uint32_t*)buffer;
	
	for (uint32_t j = 0; j < sector; j++){
		wait_BSY();
		wait_DRQ();

		for(uint32_t i = 0; i < 256; i++){
			outl(0x1F0, tmp[i]);
		}

		outb(0x1F7, 0xE7);
		
		tmp += 256;
	}
}

raw_disk_info retrieve_disk_info(){
	raw_disk_info* dinfo = (raw_disk_info*)0x5200;
	return *dinfo;
}

void decode_raw_disk_info(raw_disk_info dinfo, disk_info * result){
	result -> drivetype = dinfo.bl;
	result -> sectors = dinfo.cl & 0b00111111;
	result -> cylinders = ((dinfo.cl & 0b11000000) << 2) | dinfo.ch;
	result -> heads = dinfo.dh;
	result -> drives = dinfo.dl;
}

#define PIT_CHANNEL0_PORT 0x40
#define PIT_COMMAND_PORT 0x43

// Function to initialize PIT
void init_pit() {
	outb(0x36, PIT_COMMAND_PORT); // Set command byte: channel 0, mode 3, binary counter
}

// Function to wait for a certain number of milliseconds
void wait_ms(unsigned int milliseconds) {
	unsigned int count = 11932 * milliseconds; // PIT frequency is approximately 11932 Hz

	// Write initial count value
	outb(count & 0xFF, PIT_CHANNEL0_PORT); // Low byte
	outb((count >> 8) & 0xFF, PIT_CHANNEL0_PORT); // High byte

	// Wait until timer reaches 0
	while (count > 0) {
		// You may want to include some kind of yield or sleep mechanism here to avoid busy-waiting
		count--;
	}
}


// Play sound using built-in speaker
static void playSound(uint32_t nFrequence) {
	uint32_t Div;
	uint8_t tmp;
	// Set the PIT to the desired frequency
	Div = 1193180 / nFrequence;
	outb(0x43, 0xb6);
	outb(0x42, (uint8_t) (Div) );
	outb(0x42, (uint8_t) (Div >> 8));
	// And play the sound using the PC speaker
	tmp = inb(0x61);
 	if (tmp != (tmp | 3)) {
		outb(0x61, tmp | 3);
	}
}

//make it shut up
static void shutup() {
	uint8_t tmp = inb(0x61) & 0xFC;
	outb(0x61, tmp);
}

// Make a beep
void beep(uint32_t pitch, uint32_t ms) {
	playSound(pitch);
	wait_ms(ms);
	shutup();
}

uint32_t memcmp(const void *s1, const void *s2, size_t n) {
	const unsigned char *p1 = s1, *p2 = s2;
	while (n--) {
		if (*p1 != *p2)
			return *p1 - *p2;
		p1++, p2++;
	}
	return 0;
}

char* strpbrk(const char* s, const char* accept) {
	if (s == NULL || accept == NULL)
		return NULL;

	while (*s != '\0') {
		const char* p = accept;
		while (*p != '\0') {
			if (*s == *p)
				return (char*)s;
			p++;
		}
		s++;
	}

	return NULL;
}

char* strtok(char* str, const char* delim) {
	static char* token = NULL;
	if (str != NULL) {
		token = str;
	} else if (token == NULL) {
		return NULL;
	}

	char* start = token;
	char* end = strpbrk(token, delim);
	if (end != NULL) {
		*end = '\0';
		token = end + 1;
	} else {
		token = NULL;
	}

	return start;
}

char* strcpy(register char *to, register const char *from) {
	char *save = to;
	for (int k = 0; from[k] != '\0'; ++k) to[k] = from[k];
	return(save);
}

char* strdup(const char* s) {
	if (s == NULL)
		return NULL;

	size_t len = strlen(s);
	char* dup = (char*)malloc(len + 1); // Allocate memory for the duplicated string
	if (dup == NULL)
		return NULL;

	strcpy(dup, s); // Copy the contents of the original string into the new string
	return dup;
}

#define SECTOR_SIZE 512
#define ROOT_DIR_SECTOR 2
#define RESERVED_SECTORS 32
#define FAT_SECTORS 32

// Structure for FAT32 boot sector
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

FAT32BootSector read_boot_sector(uint8_t drive) {
	FAT32BootSector boot_sector;
	memset(&boot_sector, 0, sizeof(FAT32BootSector)); // Initialize struct with zeros
	
	LBA28_read_sector(drive, 1, 1, (uint16_t*)&boot_sector); // Corrected pointer cast
	
	return boot_sector;
}

void free_boot_sector(FAT32BootSector* boot_sector) {
	free(boot_sector);
}

// Read a sector from the disk
void read_sector(uint8_t drive, uint32_t sector, uint16_t* buffer) {
	LBA28_read_sector(drive, sector, 1, buffer);
}

// Write a sector to the disk
void write_sector(uint8_t drive, uint32_t sector, uint16_t* data) {
	LBA28_write_sector(drive, sector, 1, data);
}

// Read the FAT table entry for a given cluster
uint32_t read_fat_entry(uint8_t drive, FAT32BootSector* boot_sector, uint32_t cluster) {
	uint32_t fat_sector = boot_sector->reserved_sectors + (cluster * 4) / boot_sector->bytes_per_sector;
	uint16_t* fat_buffer = (uint16_t*)malloc(SECTOR_SIZE);
	read_sector(drive, fat_sector, fat_buffer);
	uint32_t fat_offset = (cluster * 4) % boot_sector->bytes_per_sector;
	uint32_t fat_entry = *((uint32_t*)(fat_buffer + fat_offset));
	free(fat_buffer);
	return fat_entry;
}

// Write to a FAT table entry
void write_fat_entry(uint8_t drive, FAT32BootSector* boot_sector, uint32_t cluster, uint32_t value) {
	uint32_t fat_sector = boot_sector->reserved_sectors + (cluster * 4) / boot_sector->bytes_per_sector;
	uint16_t* fat_buffer = (uint16_t*)malloc(SECTOR_SIZE);
	read_sector(drive, fat_sector, fat_buffer);
	uint32_t fat_offset = (cluster * 4) % boot_sector->bytes_per_sector;
	*((uint32_t*)(fat_buffer + fat_offset)) = value;
	write_sector(drive, fat_sector, fat_buffer);
	free(fat_buffer);
}

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

void read_file(uint8_t drive, FAT32BootSector* boot_sector, const char* filepath) {
	uint32_t cluster = boot_sector->root_cluster;
	DirectoryEntry dir_entry;

	// Tokenize the filepath to extract directory names
	char* token = strtok((char*)filepath, "/");
	while (token != NULL) {
		// Traverse the directory tree to find the directory
		while (cluster < 0x0FFFFFF8) {
			uint32_t sector = boot_sector->reserved_sectors + boot_sector->fat_count * boot_sector->sectors_per_fat + (cluster - 2) * boot_sector->sectors_per_cluster;
			uint16_t sector_buffer[SECTOR_SIZE];
			read_sector(drive, sector, sector_buffer);
			for (uint32_t i = 0; i < boot_sector->sectors_per_cluster * boot_sector->bytes_per_sector / sizeof(DirectoryEntry); i++) {
				memcpy(&dir_entry, sector_buffer + i * sizeof(DirectoryEntry), sizeof(DirectoryEntry));
				if (memcmp(token, dir_entry.filename, strlen(token)) == 0) {
					// Directory found, update the cluster and break out of the loop
					cluster = (dir_entry.first_cluster_high << 16) | dir_entry.first_cluster_low;
					break;
				}
			}
			cluster = read_fat_entry(drive, boot_sector, cluster);
		}
		token = strtok(NULL, "/");
	}

	// At this point, cluster should point to the directory containing the target file
	// Now, search for the file within this directory
	while (cluster < 0x0FFFFFF8) {
		uint32_t sector = boot_sector->reserved_sectors + boot_sector->fat_count * boot_sector->sectors_per_fat + (cluster - 2) * boot_sector->sectors_per_cluster;
		uint16_t sector_buffer[SECTOR_SIZE];
		read_sector(drive, sector, sector_buffer);
		for (uint32_t i = 0; i < boot_sector->sectors_per_cluster * boot_sector->bytes_per_sector / sizeof(DirectoryEntry); i++) {
			memcpy(&dir_entry, sector_buffer + i * sizeof(DirectoryEntry), sizeof(DirectoryEntry));
			if (memcmp(filepath, dir_entry.filename, strlen(filepath)) == 0) {
				// File found
				uint32_t file_cluster = (dir_entry.first_cluster_high << 16) | dir_entry.first_cluster_low;
				uint8_t* file_data = (uint8_t*)malloc(dir_entry.file_size);
				if (file_data == NULL) {
					putsc("Memory allocation error\n", VGA_COLOR_LIGHT_RED);
					return;
				}
				memset(file_data, 0, dir_entry.file_size);
				uint8_t* file_data_start = file_data; // Store the base address
				while (file_cluster < 0x0FFFFFF8) {
					uint32_t sector = boot_sector->reserved_sectors + boot_sector->fat_count * boot_sector->sectors_per_fat +
									  (file_cluster - 2) * boot_sector->sectors_per_cluster;
					read_sector(drive, sector, (uint16_t*)file_data);
					file_data += boot_sector->sectors_per_cluster * boot_sector->bytes_per_sector;
					file_cluster = read_fat_entry(drive, boot_sector, file_cluster);
				}
				// Print or process the file data as needed
				// Here, we'll just print the first few bytes
				for (uint32_t j = 0; j < 100 && j < dir_entry.file_size; j++) {
					putc(file_data_start[j]);
				}
				free(file_data_start); // Free the original pointer
				return;
			}
		}
		cluster = read_fat_entry(drive, boot_sector, cluster);
	}
	putsc("File not found: ", VGA_COLOR_LIGHT_RED);
	puts(filepath);
	putc('\n');
}

void list_files(uint8_t drive, FAT32BootSector* boot_sector) {
	uint32_t cluster = boot_sector->root_cluster;
	puth(cluster>>24);
	puth(cluster>>16);
	puth(cluster>>8);
	puth(cluster);
	DirectoryEntry dir_entry;
	puts("\nlisting\n");
	// Traverse the directory tree to find the directory
	while (cluster < 0x0FFFFFF8) {
		uint32_t sector = boot_sector->reserved_sectors + boot_sector->fat_count * boot_sector->sectors_per_fat + (cluster - 2) * boot_sector->sectors_per_cluster;
		uint16_t sector_buffer[SECTOR_SIZE];
		read_sector(drive, sector, sector_buffer);
		for (uint32_t i = 0; i < boot_sector->sectors_per_cluster * boot_sector->bytes_per_sector / sizeof(DirectoryEntry); i++) {
			memcpy(&dir_entry, sector_buffer + i * sizeof(DirectoryEntry), sizeof(DirectoryEntry));
			putsc(dir_entry.filename, VGA_COLOR_LIGHT_MAGENTA);
		}
		cluster = read_fat_entry(drive, boot_sector, cluster);
	}
}
