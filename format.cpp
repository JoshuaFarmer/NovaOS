#include <fstream>
#include <vector>

// Define the structure of a pointer
struct Pointer {
    uint8_t Sp; // meta sector pointer
    uint8_t Cl; // cylinder/track
    uint8_t Hd; // head
    uint8_t Dr; // drive number
};

// Define the structure of the file system start
struct FSStart {
    char id[8]; // "WFSE V1\0"
    uint16_t C; // number of file pointer sectors
};

void format_disk_image(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);

    // Skip the first 4 sectors
    file.seekp(4 * 512);

    // Write the file system start
    FSStart fs_start = {"WFSE V1", 256};
    file.write(reinterpret_cast<char*>(&fs_start), sizeof(fs_start));

    // Write the file pointers
    std::vector<Pointer> pointers(fs_start.C);
    for (const auto& pointer : pointers) {
        file.write(reinterpret_cast<const char*>(&pointer), sizeof(pointer));
    }

    file.close();
}

int main() {
    format_disk_image("disk.img");
    return 0;
}
