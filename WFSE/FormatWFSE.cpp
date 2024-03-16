#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Define constants
const int SECTOR_SIZE = 512;
const int MAX_FILES_PER_SECTOR = SECTOR_SIZE / sizeof(uint32_t);

// Structure for file entry
struct FileEntry {
    uint32_t start;
    uint32_t end;
    string name;
    // Add more metadata fields as needed
};

// Function to format disk to WFSE
void formatDisk(const string& path, int numFilePointerSectors) {
    // Open disk image file in binary write mode
    ofstream diskFile(path, ios::binary);
    if (!diskFile.is_open()) {
        cerr << "Error: Unable to open disk image file." << endl;
        return;
    }

    // Write WFSE metadata to disk
    diskFile.write("WFSE V1", 8); // Magic number
    diskFile.write(reinterpret_cast<const char*>(&numFilePointerSectors), sizeof(uint32_t)); // Number of file pointer sectors

    // Calculate total number of file pointer entries
    int totalFilePointerEntries = numFilePointerSectors * MAX_FILES_PER_SECTOR;

    // Write empty file pointer entries to disk
    vector<uint32_t> emptyFilePointers(MAX_FILES_PER_SECTOR, 0);
    for (int i = 0; i < totalFilePointerEntries; ++i) {
        diskFile.write(reinterpret_cast<const char*>(emptyFilePointers.data()), SECTOR_SIZE);
    }

    // Close disk image file
    diskFile.close();

    cout << "Disk formatted successfully." << endl;
}

int main() {
    // Example usage: format disk with 2 file pointer sectors
    formatDisk("disk.img", 2);

    return 0;
}
