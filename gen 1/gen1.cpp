#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cmath>
#include <optional>
#include "pokemon.cpp"


void extractStatsFromLine(const char *line, char *name, unsigned char *number, unsigned char *index, unsigned char *type1, unsigned char *type2, uint32_t *base_total, unsigned char *hp, unsigned char *attack, unsigned char *defense, unsigned char *speed, unsigned char *special, unsigned char *evolution) {
    // Helper function for intializePokemonWithname
    sscanf(line, "%[^,],%hhu,%hhu,%hhu,%hhu,%u,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu", name, number, index, type1, type2, base_total, hp, attack, defense, special, speed, evolution);
}

unsigned char calcStat(unsigned char base, unsigned char iv, uint16_t ev, unsigned char level){
    // Helper function for intializePokemonWithname, calculates stat(not hp) based on base stats, levels, and ivs
    int stat = ((base+iv)*2 + (sqrt(ev)/4)) * level;
    stat /= 100;
    stat += 5;
    return stat;

}

unsigned char calcHP(unsigned char base, unsigned char iv, uint16_t ev, unsigned char level){
    // Helper function for intializePokemonWithName, calculates hp based on base stats, levels, and ivs
    int stat = ((base+iv)*2 + (sqrt(ev)/4)) * level;
    stat /= 100;
    stat += level;
    stat += 10;
    return stat;
}


Pokemon initializePokemonWithName(const char *filename, 
    const char *pokemonName, 
    unsigned char level, 
    std::string move1_str, 
    std::string move2_str, 
    std::string move3_str, 
    std::string move4_str, 
    uint16_t ot_id, 
    uint32_t exp,
    uint16_t hp_ev,
    uint16_t atk_ev,
    uint16_t def_ev,
    uint16_t spe_ev, //special
    uint16_t spd_ev, //speed
    unsigned char atk_iv, // 0 - 15
    unsigned char def_iv, // 0 - 15
    unsigned char spd_iv, // 0 - 15
    unsigned char spe_iv, // 0 - 15
    unsigned char move1pp,
    unsigned char move2pp,
    unsigned char move3pp,
    unsigned char move4pp){

    //name must be upper case first letter and lower case rest
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        Pokemon p;
        return p;
    }

    

    //iv calculations
    uint16_t iv =  (atk_iv << 12) | (def_iv << 8) | (spd_iv << 4) | spe_iv;
    unsigned char hp_iv = ((atk_iv & 1) << 3) | ((def_iv & 1) << 2) | ((spd_iv & 1) << 1) | (spe_iv & 1);


    //converting moves to index
    unsigned char move1,move2,move3,move4;
    std::unordered_map<std::string, unsigned char> moves = MoveHash();
    move1 = moves[move1_str];
    move2 = moves[move2_str];
    move3 = moves[move3_str];
    move4 = moves[move4_str];

    //reading from file
    char line[50];
    char name[15];

    fgets(line, sizeof(line), file);// skip header

    while (fgets(line, sizeof(line), file)) {
        unsigned char number,index,type1,type2,hp,attack,defense,speed,special,evolution;
        uint32_t base_total;
        extractStatsFromLine(line,name,&number,&index,&type1,&type2,&base_total,&hp,&attack,&defense,&speed,&special,&evolution);
        if (strcmp(name,pokemonName) == 0){
            Pokemon p;
            p.index = index;
            p.type1 = type1;
            p.type2 = type2;
            p.level = level;
            p.status = 0;
            p.catchrate_holdItems = 0;
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
            p.level2 = level;
            p.max_hp = calcHP(hp,hp_iv,hp_ev,level);
            p.hp = p.max_hp;
            p.atk = calcStat(attack,atk_iv,atk_ev,level);
            p.def = calcStat(attack,atk_iv,atk_ev,level);
            p.spd = calcStat(attack,atk_iv,atk_ev,level);
            p.spe = calcStat(attack,atk_iv,atk_ev,level);
            return p;
        }
    }
    printf("Pokemon Not Found");
    Pokemon p;
    return p;
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
        unsigned char box_checksum = calc_checksum(filePath,start,end);
        saveFile.seekp(box_checksum_pointer,std::ios_base::beg);
        saveFile.write(reinterpret_cast<const char*>(&box_checksum),sizeof(box_checksum));
        start += 0x462;
        end += 0x462;
        box_checksum_pointer += 1;
    }
}


uint16_t reverseByteOrder(uint16_t value){
    //helper function that reverses byte order of two byte values for writing
    return (value >> 8) | (value << 8);
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
    exp[2] = mon.exp & 0xFF;
    exp[1] = (mon.exp >> 8) & 0xFF;
    exp[0] = (mon.exp >> 16) & 0xFF; 

    //reversing byte order for writing to file
    mon.hp = reverseByteOrder(mon.hp);
    mon.ot_id = reverseByteOrder(mon.ot_id);
    mon.hp_ev = reverseByteOrder(mon.hp_ev);
    mon.atk_ev = reverseByteOrder(mon.atk_ev);
    mon.def_ev = reverseByteOrder(mon.def_ev);
    mon.spd_ev = reverseByteOrder(mon.spd_ev);
    mon.spe_ev = reverseByteOrder(mon.spe_ev);
    mon.iv = reverseByteOrder(mon.iv);
    mon.max_hp = reverseByteOrder(mon.max_hp);
    mon.atk = reverseByteOrder(mon.atk);
    mon.def = reverseByteOrder(mon.def);
    mon.spd = reverseByteOrder(mon.spd);
    mon.spe = reverseByteOrder(mon.spe);

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
    writeFile.write(reinterpret_cast<char*>(&(mon.spe)),sizeof(byte)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.spd)),sizeof(byte)*2);
    
    
    //writing trainer name
    readFile.seekg(0x303C,std::ios::beg);
    char name[11];
    readFile.read(reinterpret_cast<char*>(&name),11);
    writeFile.seekp(0xB*byte,std::ios::cur);
    writeFile.write(reinterpret_cast<char*>(&name),11);

 
    //writing pokemon nickname(as "A")
    writeFile.seekp(0xB*byte,std::ios::cur);
    unsigned char nickname = 0x80;
    writeFile.write(reinterpret_cast<char*>(&nickname),1);
    unsigned char end = 0x50;
    for(int i=0;i<10;i++){
        writeFile.write(reinterpret_cast<char*>(&end),1);
    }

    readFile.close();
    writeFile.close();
}




int main(){
    std::string dataPath = "FirstGenPokemon.csv";
    const char* cdataPath = dataPath.c_str();
    // Pokemon bulbasaur = initializePartyPokemon(153,222,100,0,22,3,0,22,0,0,0,17720,1059860,1594,1673,1809,1092,1595,0xFFFF,10,0,0,0,100,222,137,135,167,131);
    Pokemon bulbasaur = initializePokemonWithName(cdataPath,"Bulbasaur",100,"Vine Whip","Mega Drain","None","none",17720,1059860,1594,1673,1809,1092,1595,15,15,15,15,10,10,0,0);
    std::string filePath = "save_deleted - Copy.sav";
    addPokemonToParty(filePath,bulbasaur);
    write_main_checksum(filePath);
    write_box_checksums(filePath);

    
    return 0;
}

