#include <cstring>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <cmath>


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

 std::unordered_map<std::string, unsigned char> MoveHash(){
    //Words in moves must be capatalized
    std::unordered_map<std::string, unsigned char> moveToId;
    moveToId["None"] = 0;
    moveToId["none"] = 0;
    moveToId["Pound"] = 1;
    moveToId["Karate Chop"] = 2;
    moveToId["Double Slap"] = 3;
    moveToId["Comet Punch"] = 4;
    moveToId["Mega Punch"] = 5;
    moveToId["Pay Day"] = 6;
    moveToId["Fire Punch"] = 7;
    moveToId["Ice Punch"] = 8;
    moveToId["Thunder Punch"] = 9;
    moveToId["Scratch"] = 10;
    moveToId["Vice Grip"] = 11;
    moveToId["Guillotine"] = 12;
    moveToId["Razor Wind"] = 13;
    moveToId["Swords Dance"] = 14;
    moveToId["Cut"] = 15;
    moveToId["Gust"] = 16;
    moveToId["Wing Attack"] = 17;
    moveToId["Whirlwind"] = 18;
    moveToId["Fly"] = 19;
    moveToId["Bind"] = 20;
    moveToId["Slam"] = 21;
    moveToId["Vine Whip"] = 22;
    moveToId["Stomp"] = 23;
    moveToId["Double Kick"] = 24;
    moveToId["Mega Kick"] = 25;
    moveToId["Jump Kick"] = 26;
    moveToId["Rolling Kick"] = 27;
    moveToId["Sand Attack"] = 28;
    moveToId["Headbutt"] = 29;
    moveToId["Horn Attack"] = 30;
    moveToId["Fury Attack"] = 31;
    moveToId["Horn Drill"] = 32;
    moveToId["Tackle"] = 33;
    moveToId["Body Slam"] = 34;
    moveToId["Wrap"] = 35;
    moveToId["Take Down"] = 36;
    moveToId["Thrash"] = 37;
    moveToId["Double-Edge"] = 38;
    moveToId["Tail Whip"] = 39;
    moveToId["Poison Sting"] = 40;
    moveToId["Twineedle"] = 41;
    moveToId["Pin Missile"] = 42;
    moveToId["Leer"] = 43;
    moveToId["Bite"] = 44;
    moveToId["Growl"] = 45;
    moveToId["Roar"] = 46;
    moveToId["Sing"] = 47;
    moveToId["Supersonic"] = 48;
    moveToId["Sonic Boom"] = 49;
    moveToId["Disable"] = 50;
    moveToId["Acid"] = 51;
    moveToId["Ember"] = 52;
    moveToId["Flamethrower"] = 53;
    moveToId["Mist"] = 54;
    moveToId["Water Gun"] = 55;
    moveToId["Hydro Pump"] = 56;
    moveToId["Surf"] = 57;
    moveToId["Ice Beam"] = 58;
    moveToId["Blizzard"] = 59;
    moveToId["Psybeam"] = 60;
    moveToId["Bubble Beam"] = 61;
    moveToId["Aurora Beam"] = 62;
    moveToId["Hyper Beam"] = 63;
    moveToId["Peck"] = 64;
    moveToId["Drill Peck"] = 65;
    moveToId["Submission"] = 66;
    moveToId["Low Kick"] = 67;
    moveToId["Counter"] = 68;
    moveToId["Seismic Toss"] = 69;
    moveToId["Strength"] = 70;
    moveToId["Absorb"] = 71;
    moveToId["Mega Drain"] = 72;
    moveToId["Leech Seed"] = 73;
    moveToId["Growth"] = 74;
    moveToId["Razor Leaf"] = 75;
    moveToId["Solar Beam"] = 76;
    moveToId["Poison Powder"] = 77;
    moveToId["Stun Spore"] = 78;
    moveToId["Sleep Powder"] = 79;
    moveToId["Petal Dance"] = 80;
    moveToId["String Shot"] = 81;
    moveToId["Dragon Rage"] = 82;
    moveToId["Fire Spin"] = 83;
    moveToId["Thunder Shock"] = 84;
    moveToId["Thunderbolt"] = 85;
    moveToId["Thunder Wave"] = 86;
    moveToId["Thunder"] = 87;
    moveToId["Rock Throw"] = 88;
    moveToId["Earthquake"] = 89;
    moveToId["Fissure"] = 90;
    moveToId["Dig"] = 91;
    moveToId["Toxic"] = 92;
    moveToId["Confusion"] = 93;
    moveToId["Psychic"] = 94;
    moveToId["Hypnosis"] = 95;
    moveToId["Meditate"] = 96;
    moveToId["Agility"] = 97;
    moveToId["Quick Attack"] = 98;
    moveToId["Rage"] = 99;
    moveToId["Teleport"] = 100;
    moveToId["Night Shade"] = 101;
    moveToId["Mimic"] = 102;
    moveToId["Screech"] = 103;
    moveToId["Double Team"] = 104;
    moveToId["Recover"] = 105;
    moveToId["Harden"] = 106;
    moveToId["Minimize"] = 107;
    moveToId["Smoke Screen"] = 108;
    moveToId["Confuse Ray"] = 109;
    moveToId["Withdraw"] = 110;
    moveToId["Defense Curl"] = 111;
    moveToId["Barrier"] = 112;
    moveToId["Light Screen"] = 113;
    moveToId["Haze"] = 114;
    moveToId["Reflect"] = 115;
    moveToId["Focus Energy"] = 116;
    moveToId["Bide"] = 117;
    moveToId["Metronome"] = 118;
    moveToId["Mirror Move"] = 119;
    moveToId["Self-Destruct"] = 120;
    moveToId["Egg Bomb"] = 121;
    moveToId["Lick"] = 122;
    moveToId["Smog"] = 123;
    moveToId["Sludge"] = 124;
    moveToId["Bone Club"] = 125;
    moveToId["Fire Blast"] = 126;
    moveToId["Waterfall"] = 127;
    moveToId["Clamp"] = 128;
    moveToId["Swift"] = 129;
    moveToId["Skull Bash"] = 130;
    moveToId["Spike Cannon"] = 131;
    moveToId["Constrict"] = 132;
    moveToId["Amnesia"] = 133;
    moveToId["Kinesis"] = 134;
    moveToId["Soft-Boiled"] = 135;
    moveToId["Hi Jump Kick"] = 136;
    moveToId["Glare"] = 137;
    moveToId["Dream Eater"] = 138;
    moveToId["Poison Gas"] = 139;
    moveToId["Barrage"] = 140;
    moveToId["Leech Life"] = 141;
    moveToId["Lovely Kiss"] = 142;
    moveToId["Sky Attack"] = 143;
    moveToId["Transform"] = 144;
    moveToId["Bubble"] = 145;
    moveToId["Dizzy Punch"] = 146;
    moveToId["Spore"] = 147;
    moveToId["Flash"] = 148;
    moveToId["Psywave"] = 149;
    moveToId["Splash"] = 150;
    moveToId["Acid Armor"] = 151;
    moveToId["Crabhammer"] = 152;
    moveToId["Explosion"] = 153;
    moveToId["Fury Swipes"] = 154;
    moveToId["Bonemerang"] = 155;
    moveToId["Rest"] = 156;
    moveToId["Rock Slide"] = 157;
    moveToId["Hyper Fang"] = 158;
    moveToId["Sharpen"] = 159;
    moveToId["Conversion"] = 160;
    moveToId["Tri Attack"] = 161;
    moveToId["Super Fang"] = 162;
    moveToId["Slash"] = 163;
    moveToId["Substitute"] = 164;
    moveToId["Struggle"] = 165;
    return moveToId;
 }

uint16_t reverseByteOrder(uint16_t value){
    //helper function that reverses byte order of two byte values for writing
    return (value >> 8) | (value << 8);
}

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