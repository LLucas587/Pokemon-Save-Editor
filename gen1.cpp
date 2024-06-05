#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

typedef struct {
    unsigned char index;
    uint8_t hp;
    unsigned char level;
    unsigned char status;
    unsigned char type1;
    unsigned char type2;
    unsigned char move1;
    unsigned char move2;
    unsigned char move3;
    unsigned char move4;
    uint8_t ot_id;
    uint32_t exp; //only use 3 bytes
    uint8_t hp_ev;
    uint8_t atk_ev;
    uint8_t def_ev;
    uint8_t spd_ev; //speed
    uint8_t spe_ev; //special
    uint8_t iv;
    unsigned char move1pp;
    unsigned char move2pp;
    unsigned char move3pp;
    unsigned char move4pp;
    unsigned char level2;
    uint8_t max_hp;
    uint8_t atk;
    uint8_t def;
    uint8_t spd;
    uint8_t spe;
} Pokemon;

void edit(const std::string& filePath) {
    std::fstream saveFile(filePath, std::ios::in | std::ios::out | std::ios::binary);
    if (!saveFile.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
    }
}

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
    printf("%d\n",checksum);
    return checksum;
}



void write_main_checksum(const std::string& filePath){//working
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
    saveFile.seekp(0x5A4C,std::ios_base::beg); // culumative box checksum
    const char new_checksum = calc_checksum(filePath,0x4000,0x5A4B);
    saveFile.write(&new_checksum,sizeof(new_checksum));
    
    
    //individual box checksums
    uint32_t start = 0x4000;
    uint32_t end = 0x4461;
    uint32_t box_checksum_pointer = 0x5A4D;
    for(int i=0;i<6;i++){
        printf("%04x %04x\n",start,end);
        unsigned char box_checksum = calc_checksum(filePath,start,end);
        saveFile.seekp(box_checksum_pointer,std::ios_base::beg);
        saveFile.write(reinterpret_cast<const char*>(&box_checksum),sizeof(box_checksum));
        start += 0x462;
        end += 0x462;
        box_checksum_pointer += 1;
    }

    saveFile.seekp(0x7A4C,std::ios_base::beg); // culumative box checksum
    const char new_checksum_2 = calc_checksum(filePath,0x6000,0x7A4B);
    saveFile.write(&new_checksum_2,sizeof(new_checksum_2));


    start = 0x6000;
    end = 0x6461;
    box_checksum_pointer = 0x7A4D;
    for(int i=0;i<6;i++){
        printf("%04x %04x\n",start,end);
        unsigned char box_checksum = calc_checksum(filePath,start,end);
        saveFile.seekp(box_checksum_pointer,std::ios_base::beg);
        saveFile.write(reinterpret_cast<const char*>(&box_checksum),sizeof(box_checksum));
        start += 0x462;
        end += 0x462;
        box_checksum_pointer += 1;
    }
}

void addPokemonToParty(const std::string& filePath){
    std::ifstream readFile(filePath, std::ios::in | std::ios::out | std::ios::binary);
    if (!readFile.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return;
    }
    std::ofstream writeFile(filePath, std::ios::in | std::ios::out | std::ios::binary);
    if (!writeFile.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return;
    }

    //rewriting number of party pokemon
    unsigned char byte;
    readFile.seekg(0x2F2C, std::ios::beg);
    writeFile.seekp(0x2F2C, std::ios::beg);
    readFile.read(reinterpret_cast<char*>(&byte), sizeof(byte));
    if (readFile.gcount() == 0) {
        std::cerr << "Error: Could not read byte " << std::endl;
        return;
    }
    if(byte<6){
        byte = byte + 1;
        writeFile.write(reinterpret_cast<char*>(&byte),sizeof(byte));
    }


}

void editPartyData(const std::string& filePath){
    std::fstream saveFile(filePath, std::ios::in | std::ios::out | std::ios::binary);

}



int main(){
    std::string filePath = "save.sav";
    write_main_checksum(filePath);
    write_box_checksums(filePath);
    return 0;
}

