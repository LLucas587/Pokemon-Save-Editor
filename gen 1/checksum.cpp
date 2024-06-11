#include <iostream>
#include <fstream>
#include <cstring>

unsigned char calc_checksum(const std::string& filePath, std::streampos start, std::streampos end){
    std::ifstream saveFile(filePath, std::ios::in | std::ios::out | std::ios::binary);
    if (!saveFile.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return 0;
    }

    unsigned char checksum = 255;
    saveFile.seekg(start, std::ios::beg);
    for (std::streampos pos = start; pos <= end; pos += 1) {
        unsigned char byte;
        saveFile.read(reinterpret_cast<char*>(&byte), sizeof(byte));
        if (saveFile.gcount() == 0) {
            std::cerr << "Error: Could not read byte at position " << pos << std::endl;
            break;
        }   
        checksum -= byte;
    }

    saveFile.close();
    return checksum;
}



void write_main_checksum(const std::string& filePath){
    std::fstream saveFile(filePath, std::ios::in | std::ios::out | std::ios::binary);
    if (!saveFile.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return;
    }
    saveFile.seekp(0x3523,std::ios_base::beg);
    const char new_checksum = calc_checksum(filePath,0x2598,0x3522);
    saveFile.write(&new_checksum,sizeof(new_checksum));
    saveFile.close();
}

void write_box_checksums(const std::string& filePath){
    std::fstream saveFile(filePath, std::ios::in | std::ios::out | std::ios::binary);
    if (!saveFile.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return;
    }
    saveFile.seekp(0x5A4C,std::ios_base::beg); // 1st culumative box checksum
    const char new_checksum = calc_checksum(filePath,0x4000,0x5A4B);
    saveFile.write(&new_checksum,sizeof(new_checksum));
    
    
    //individual box checksums 1-6
    uint32_t start = 0x4000;
    uint32_t end = 0x4461;
    uint32_t box_checksum_pointer = 0x5A4D;
    for(int i=0;i<6;i++){
        unsigned char box_checksum = calc_checksum(filePath,start,end);
        saveFile.seekp(box_checksum_pointer,std::ios_base::beg);
        saveFile.write(reinterpret_cast<const char*>(&box_checksum),sizeof(box_checksum));
        start += 0x462;
        end += 0x462;
        box_checksum_pointer += 1;
    }

    saveFile.seekp(0x7A4C,std::ios_base::beg); // 2nd culumative box checksum
    const char new_checksum_2 = calc_checksum(filePath,0x6000,0x7A4B);
    saveFile.write(&new_checksum_2,sizeof(new_checksum_2));

    //individual box checksums 7-12
    start = 0x6000;
    end = 0x6461;
    box_checksum_pointer = 0x7A4D;
    for(int i=0;i<6;i++){
        unsigned char box_checksum = calc_checksum(filePath,start,end);
        saveFile.seekp(box_checksum_pointer,std::ios_base::beg);
        saveFile.write(reinterpret_cast<const char*>(&box_checksum),sizeof(box_checksum));
        start += 0x462;
        end += 0x462;
        box_checksum_pointer += 1;
    }
}