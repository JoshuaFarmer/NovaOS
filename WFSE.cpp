#include <iostream>
#include <fstream>
#include <vector>

#define sector_size 512
#define disk_size   512*1024

struct file {
	uint32_t start; 
	uint32_t end; 
	std::vector<byte> data; 
};

void WFSE_Format(std::string path, struct file) {

}