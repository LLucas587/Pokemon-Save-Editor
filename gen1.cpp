#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

typedef struct {
    unsigned char index; // not pokemon id number in gen 1
    uint16_t hp;
    unsigned char level;
    unsigned char status;
    unsigned char type1;
    unsigned char type2;
    unsigned char catchrate_holdItems;
    unsigned char move1;
    unsigned char move2;
    unsigned char move3;
    unsigned char move4;
    uint16_t ot_id;
    uint32_t exp; //only use 3 bytes
    uint16_t hp_ev;
    uint16_t atk_ev;
    uint16_t def_ev;
    uint16_t spe_ev; //special
    uint16_t spd_ev; //speed
    uint16_t iv;
    unsigned char move1pp;
    unsigned char move2pp;
    unsigned char move3pp;
    unsigned char move4pp;
    unsigned char level2;
    uint16_t max_hp;
    uint16_t atk;
    uint16_t def;
    uint16_t spd;
    uint16_t spe;
} Pokemon;

Pokemon initializePartyPokemon(unsigned char index, 
    uint16_t hp, 
    unsigned char level,
    unsigned char status,
    unsigned char type1,
    unsigned char type2,
    unsigned char catchrate_holdItems,
    unsigned char move1,
    unsigned char move2,
    unsigned char move3,
    unsigned char move4,
    uint16_t ot_id,
    uint32_t exp, //only use 3 bytes
    uint16_t hp_ev,
    uint16_t atk_ev,
    uint16_t def_ev,
    uint16_t spe_ev, //special
    uint16_t spd_ev, //speed
    uint16_t iv,
    unsigned char move1pp,
    unsigned char move2pp,
    unsigned char move3pp,
    unsigned char move4pp,
    unsigned char level2,
    uint16_t max_hp,
    uint16_t atk,
    uint16_t def,
    uint16_t spd,
    uint16_t spe){
        Pokemon p;
        p.index = index;
        p.hp = hp;
        p.level = level;
        p.status = status;
        p.type1 = type1;
        p.type2 = type2;
        p.catchrate_holdItems =  catchrate_holdItems;
        p.move1 = move1;
        p.move2 = move2;
        p.move3 = move3;
        p.move4 = move4;
        p.ot_id = ot_id;
        p.exp = exp & 0xFFFFFF; // only use 3 lower bytes
        p.hp_ev = hp_ev;
        p.atk_ev = atk_ev;
        p.def_ev = def_ev;
        p.spd_ev = spd_ev;
        p.spe_ev = spe_ev;
        p.iv = iv;
        p.move1pp = move1pp;
        p.move2pp = move2pp;
        p.move3pp = move3pp;
        p.move4pp = move4pp;
        p.level2 = level2;
        p.max_hp = max_hp;
        p.atk = atk;
        p.def = def;
        p.spd = spd;
        p.spe = spe;
        return p;
    }

Pokemon initializeBoxPokemon(unsigned char index, 
    uint16_t hp, 
    unsigned char level,
    unsigned char status,
    unsigned char type1,
    unsigned char type2,
    unsigned char catchrate_holdItems,
    unsigned char move1,
    unsigned char move2,
    unsigned char move3,
    unsigned char move4,
    uint16_t ot_id,
    uint32_t exp, //only use 3 bytes
    uint16_t hp_ev,
    uint16_t atk_ev,
    uint16_t def_ev,
    uint16_t spe_ev, //special
    uint16_t spd_ev, //speed
    uint16_t iv,
    unsigned char move1pp,
    unsigned char move2pp,
    unsigned char move3pp,
    unsigned char move4pp){
        Pokemon p;
        p.index = index;
        p.hp = hp;
        p.level = level;
        p.status = status;
        p.type1 = type1;
        p.type2 = type2;
        p.catchrate_holdItems =  catchrate_holdItems;
        p.move1 = move1;
        p.move2 = move2;
        p.move3 = move3;
        p.move4 = move4;
        p.ot_id = ot_id;
        p.exp = exp & 0xFFFFFF; // only use 3 lower bytes
        p.hp_ev = hp_ev;
        p.atk_ev = atk_ev;
        p.def_ev = def_ev;
        p.spd_ev = spd_ev;
        p.spe_ev = spe_ev;
        p.iv = iv;
        p.move1pp = move1pp;
        p.move2pp = move2pp;
        p.move3pp = move3pp;
        p.move4pp = move4pp;
        return p;
    }   


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

void addPokemonToParty(const std::string& filePath, Pokemon mon){
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
        unsigned char amount = byte + 1;
        writeFile.write(reinterpret_cast<char*>(&amount),sizeof(byte));
    }
    else{
         std::cerr << "Error: Party is Full" << std::endl;
         return;
    }   

    //write in species id
    writeFile.seekp(byte,std::ios::cur);
    writeFile.write(reinterpret_cast<char*>(&(mon.index)),sizeof(byte));
    writeFile.seekp(1,std::ios::cur);//seeking past padding

    //seeking past other pokemon entries
    writeFile.seekp(byte*0x2C,std::ios::cur);
    
    //changing exp to 3 bytes
    unsigned char exp[3];
    exp[0] = mon.exp & 0xFF;
    exp[1] = (mon.exp >> 8) & 0xFF;
    exp[2] = (mon.exp >> 16) & 0xFF; 

    //writing pokemon stats
    writeFile.write(reinterpret_cast<char*>(&(mon.index)),sizeof(byte));
    writeFile.write(reinterpret_cast<char*>(&(mon.hp)),sizeof(byte)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.level)),sizeof(byte));
    writeFile.write(reinterpret_cast<char*>(&(mon.status)),sizeof(byte));
    writeFile.write(reinterpret_cast<char*>(&(mon.type1)),sizeof(byte));
    writeFile.write(reinterpret_cast<char*>(&(mon.type2)),sizeof(byte));
    writeFile.write(reinterpret_cast<char*>(&(mon.catchrate_holdItems)),sizeof(byte));
    writeFile.write(reinterpret_cast<char*>(&(mon.move1)),sizeof(byte));
    writeFile.write(reinterpret_cast<char*>(&(mon.move2)),sizeof(byte));
    writeFile.write(reinterpret_cast<char*>(&(mon.move3)),sizeof(byte));
    writeFile.write(reinterpret_cast<char*>(&(mon.move4)),sizeof(byte));
    writeFile.write(reinterpret_cast<char*>(&(mon.ot_id)),sizeof(byte)*2);
    writeFile.write(reinterpret_cast<char*>(&(exp)),sizeof(byte)*3);
    writeFile.write(reinterpret_cast<char*>(&(mon.hp_ev)),sizeof(byte)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.atk_ev)),sizeof(byte)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.def_ev)),sizeof(byte)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.spd_ev)),sizeof(byte)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.spe_ev)),sizeof(byte)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.iv)),sizeof(byte)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.move1pp)),sizeof(byte));
    writeFile.write(reinterpret_cast<char*>(&(mon.move2pp)),sizeof(byte));
    writeFile.write(reinterpret_cast<char*>(&(mon.move3pp)),sizeof(byte));
    writeFile.write(reinterpret_cast<char*>(&(mon.move4pp)),sizeof(byte));
    writeFile.write(reinterpret_cast<char*>(&(mon.level2)),sizeof(byte));
    writeFile.write(reinterpret_cast<char*>(&(mon.max_hp)),sizeof(byte)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.atk)),sizeof(byte)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.def)),sizeof(byte)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.spd)),sizeof(byte)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.spe)),sizeof(byte)*2);
    
    //writing trainer name(not working)
    writeFile.seekp(0xB*byte,std::ios::cur);
    writeFile.write("a",1);
    writeFile.write("0xFF",1);
 
    //writing pokemon nickname(not working)
    writeFile.seekp(0xB*byte -2,std::ios::cur);
    writeFile.write("a",1);
    writeFile.write("0x50",1);

    readFile.close();
    writeFile.close();
}

void editPartyData(const std::string& filePath){
    std::fstream saveFile(filePath, std::ios::in | std::ios::out | std::ios::binary);

}



int main(){
    Pokemon bulbasaur = initializePartyPokemon(153,222,100,0,22,3,0,22,0xFF,0xFF,0xFF,17720,1059860,1594,1673,1809,1092,1595,0xDCDD,16,0,0,0,100,222,137,135,167,131);
    std::string filePath = "save.sav";
    addPokemonToParty(filePath,bulbasaur);
    write_main_checksum(filePath);
    write_box_checksums(filePath);
    return 0;
}

