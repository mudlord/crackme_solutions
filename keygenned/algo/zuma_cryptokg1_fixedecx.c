#include <stdbool.h>
#include <stdint.h>
#include <windows.h>


void init() {}

void process_serial(char* name, char* serial_out) {
    uint8_t *lut = "XZMAJK";
    unsigned namelen = lstrlen(name);

    //the checksum hash
    uint8_t hash = namelen;
    for (unsigned i = 0; i < namelen; ++i)
    {
        hash += name[i];
        hash += 0x76;
        hash *= (namelen - i);
        hash *= 2;
    }
    //computed from 0xAF2 and 0x130 using xgcd
    unsigned part1 = hash * 23;
    unsigned part2 = hash * 212;
    //first portion
    uint8_t hash_bytes1[10] = { 0 };
    uint8_t hash_bytes2[10] = { 0 };
    for (unsigned i = 0; i < 7; i++)
    {
        hash_bytes1[6 - i] = lut[part1 % 6];
        part1 /= 6;
        hash_bytes2[6 - i] = lut[part2 % 6];
        part2 /= 6;
    }
    wsprintf(serial_out, "%s-%s",hash_bytes1,hash_bytes2);
}