#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cmath>
#include "pokemon.cpp"
#include "checksum.cpp"


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
        throw std::invalid_argument("File Not Found");
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
    throw std::invalid_argument("Pokemon Not Found");
}

Pokemon extractPokemonFromFile_Party(const std::string& filePath, int index){
    //returns a pokemon struct from reading from save file, index(1-6)
    Pokemon p;
    std::ifstream readFile(filePath, std::ios::in | std::ios::out | std::ios::binary);
    if (!readFile.is_open()) {
        throw std::invalid_argument("File Not Found");
    }
    index -= 1;
    readFile.seekg(0x2F2C+8+(0x2c*index), std::ios::beg);//seek to data of pokemon stored in index
    readFile.read(reinterpret_cast<char*>(&p.index), sizeof(unsigned char));
    readFile.read(reinterpret_cast<char*>(&p.hp), sizeof(unsigned char)*2);
    readFile.read(reinterpret_cast<char*>(&p.level), sizeof(unsigned char));
    readFile.read(reinterpret_cast<char*>(&p.status), sizeof(unsigned char));
    readFile.read(reinterpret_cast<char*>(&p.type1), sizeof(unsigned char));
    readFile.read(reinterpret_cast<char*>(&p.type2), sizeof(unsigned char));
    readFile.read(reinterpret_cast<char*>(&p.catchrate_holdItems), sizeof(unsigned char));
    readFile.read(reinterpret_cast<char*>(&p.move1), sizeof(unsigned char));
    readFile.read(reinterpret_cast<char*>(&p.move2), sizeof(unsigned char));
    readFile.read(reinterpret_cast<char*>(&p.move3), sizeof(unsigned char));
    readFile.read(reinterpret_cast<char*>(&p.move4), sizeof(unsigned char));
    readFile.read(reinterpret_cast<char*>(&p.ot_id), sizeof(unsigned char)*2);
    readFile.read(reinterpret_cast<char*>(&p.exp), sizeof(unsigned char)*3);
    readFile.read(reinterpret_cast<char*>(&p.hp_ev), sizeof(unsigned char)*2);
    readFile.read(reinterpret_cast<char*>(&p.atk_ev), sizeof(unsigned char)*2);
    readFile.read(reinterpret_cast<char*>(&p.def_ev), sizeof(unsigned char)*2);
    readFile.read(reinterpret_cast<char*>(&p.spe_ev), sizeof(unsigned char)*2);
    readFile.read(reinterpret_cast<char*>(&p.spd_ev), sizeof(unsigned char)*2);
    readFile.read(reinterpret_cast<char*>(&p.iv), sizeof(unsigned char)*2);
    readFile.read(reinterpret_cast<char*>(&p.move1pp), sizeof(unsigned char));
    readFile.read(reinterpret_cast<char*>(&p.move2pp), sizeof(unsigned char));
    readFile.read(reinterpret_cast<char*>(&p.move3pp), sizeof(unsigned char));
    readFile.read(reinterpret_cast<char*>(&p.move4pp), sizeof(unsigned char));
    readFile.read(reinterpret_cast<char*>(&p.max_hp), sizeof(unsigned char)*2);
    readFile.read(reinterpret_cast<char*>(&p.atk), sizeof(unsigned char)*2);
    readFile.read(reinterpret_cast<char*>(&p.def), sizeof(unsigned char)*2);
    readFile.read(reinterpret_cast<char*>(&p.spd), sizeof(unsigned char)*2);
    readFile.read(reinterpret_cast<char*>(&p.spe), sizeof(unsigned char)*2);

    p.hp=reverseByteOrder(p.hp);
    p.ot_id=reverseByteOrder(p.ot_id);
    p.hp_ev=reverseByteOrder(p.hp_ev);
    p.atk_ev=reverseByteOrder(p.atk_ev);
    p.def_ev=reverseByteOrder(p.def_ev);
    p.spe_ev=reverseByteOrder(p.spe_ev);
    p.iv = reverseByteOrder(p.iv);
    p.max_hp=reverseByteOrder(p.max_hp);
    p.atk=reverseByteOrder(p.atk);
    p.def=reverseByteOrder(p.def);
    p.spd=reverseByteOrder(p.spd);
    p.spe=reverseByteOrder(p.spe);

    unsigned char exp[3];
    exp[2] = p.exp & 0xFF;
    exp[1] = (p.exp >> 8) & 0xFF;
    exp[0] = (p.exp >> 16) & 0xFF; 
    p.exp = (uint32_t)exp[2] << 16 |
        (uint32_t)exp[1] << 8  |
        (uint32_t)exp[0];
    
    readFile.close();
    return p;

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
        writeFile.write(reinterpret_cast<char*>(&amount),sizeof(unsigned char));
    }
    else{
         std::cerr << "Error: Party is Full" << std::endl;
         return;
    }   

    //write in species id
    writeFile.seekp(byte,std::ios::cur);
    writeFile.write(reinterpret_cast<char*>(&(mon.index)),sizeof(unsigned char));
    writeFile.seekp(6-byte,std::ios::cur);//seeking past padding and rest of party

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
    writeFile.write(reinterpret_cast<char*>(&(mon.index)),sizeof(unsigned char));
    writeFile.write(reinterpret_cast<char*>(&(mon.hp)),sizeof(unsigned char)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.level)),sizeof(unsigned char));
    writeFile.write(reinterpret_cast<char*>(&(mon.status)),sizeof(unsigned char));
    writeFile.write(reinterpret_cast<char*>(&(mon.type1)),sizeof(unsigned char));
    writeFile.write(reinterpret_cast<char*>(&(mon.type2)),sizeof(unsigned char));
    writeFile.write(reinterpret_cast<char*>(&(mon.catchrate_holdItems)),sizeof(unsigned char));
    writeFile.write(reinterpret_cast<char*>(&(mon.move1)),sizeof(unsigned char));
    writeFile.write(reinterpret_cast<char*>(&(mon.move2)),sizeof(unsigned char));
    writeFile.write(reinterpret_cast<char*>(&(mon.move3)),sizeof(unsigned char));
    writeFile.write(reinterpret_cast<char*>(&(mon.move4)),sizeof(unsigned char));
    writeFile.write(reinterpret_cast<char*>(&(mon.ot_id)),sizeof(unsigned char)*2);
    writeFile.write(reinterpret_cast<char*>(&(exp)),sizeof(unsigned char)*3);
    writeFile.write(reinterpret_cast<char*>(&(mon.hp_ev)),sizeof(unsigned char)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.atk_ev)),sizeof(unsigned char)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.def_ev)),sizeof(unsigned char)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.spd_ev)),sizeof(unsigned char)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.spe_ev)),sizeof(unsigned char)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.iv)),sizeof(unsigned char)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.move1pp)),sizeof(unsigned char));
    writeFile.write(reinterpret_cast<char*>(&(mon.move2pp)),sizeof(unsigned char));
    writeFile.write(reinterpret_cast<char*>(&(mon.move3pp)),sizeof(unsigned char));
    writeFile.write(reinterpret_cast<char*>(&(mon.move4pp)),sizeof(unsigned char));
    writeFile.write(reinterpret_cast<char*>(&(mon.level2)),sizeof(unsigned char));
    writeFile.write(reinterpret_cast<char*>(&(mon.max_hp)),sizeof(unsigned char)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.atk)),sizeof(unsigned char)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.def)),sizeof(unsigned char)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.spe)),sizeof(unsigned char)*2);
    writeFile.write(reinterpret_cast<char*>(&(mon.spd)),sizeof(unsigned char)*2);
    
    
    //writing trainer name
    readFile.seekg(0x303C,std::ios::beg);
    char name[11];
    readFile.read(reinterpret_cast<char*>(&name),11);
    writeFile.seekp(0x303c+0xB*byte,std::ios::beg);
    writeFile.write(reinterpret_cast<char*>(&name),11);

 
    //writing pokemon nickname(as "A")
    writeFile.seekp(0x307e + 0xB*byte,std::ios::beg);
    unsigned char nickname = 0x80;
    writeFile.write(reinterpret_cast<char*>(&nickname),1);
    unsigned char end = 0x50;
    for(int i=0;i<10;i++){
        writeFile.write(reinterpret_cast<char*>(&end),1);
    }

    readFile.close();
    writeFile.close();
}

void removePokemonfromParty(const std::string& filePath,unsigned char index){
    //remove pokemon from party(index 1-6)
    //use alone only if index 6
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

    //rewriting pokemon counter
    unsigned char byte;
    readFile.seekg(0x2F2C, std::ios::beg);
    writeFile.seekp(0x2F2C, std::ios::beg);
    readFile.read(reinterpret_cast<char*>(&byte), sizeof(byte));
    if (readFile.gcount() == 0) {
        std::cerr << "Error: Could not read byte " << std::endl;
        return;
    }

    if(byte <= 6 && byte > 1){
        unsigned char amount = byte - 1;
        writeFile.write(reinterpret_cast<char*>(&amount),sizeof(byte));
    }
    else{
         std::cerr << "Error: Index out of Bounds" << std::endl;
         return;
    }   
    
    //rewriting species id
    index = index-1;
    writeFile.seekp(index,std::ios::cur);
    unsigned char max = 0xFF;
    writeFile.write(reinterpret_cast<char*>(&max),sizeof(byte));
    writeFile.seekp(1,std::ios::cur);//seeking past padding
    
    std::vector<char> zerobuffer(0x2C, 0);
    std::vector<char> charbuffer(11, 0x50);

    writeFile.seekp(index*0x2C,std::ios::cur);
    writeFile.write(zerobuffer.data(), zerobuffer.size());//deleting party data
    readFile.seekg(0x303C,std::ios::beg);
    writeFile.seekp(0xB*index,std::ios::cur);//deleting trainer name
    writeFile.write(charbuffer.data(), 11);
    writeFile.seekp(0xB*index,std::ios::cur);//deleting pokemon name
    writeFile.write(charbuffer.data(), 11);

    readFile.close();
    writeFile.close();
}






int main(){
    std::string dataPath = "FirstGenPokemon.csv";
    const char* cdataPath = dataPath.c_str();
    // Pokemon bulbasaur = initializePartyPokemon(153,222,100,0,22,3,0,22,0,0,0,17720,1059860,1594,1673,1809,1092,1595,0xFFFF,10,0,0,0,100,222,137,135,167,131);
    Pokemon bulbasaur = initializePokemonWithName(cdataPath,"Bulbasaur",100,"Vine Whip","Mega Drain","None","none",17720,1059860,1594,1673,1809,1092,1595,15,15,15,15,10,10,0,0);
    std::string filePath = "save_deleted - Copy.sav";
    // addPokemonToParty(filePath,bulbasaur);
    // removePokemonfromParty(filePath,6);
    Pokemon bulbasaur2 = extractPokemonFromFile_Party(filePath,5);
    addPokemonToParty(filePath,bulbasaur2);
    write_main_checksum(filePath);
    write_box_checksums(filePath);
    
    return 0;
}

