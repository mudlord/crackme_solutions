#include "patternfind.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    unsigned char data[0x100];
    memcpy(data, main, sizeof(data));

    //find pattern offset
    size_t found = patternfind(data, sizeof(data), "68 ?? ?1 0? 00");

    printf("found: main+%p\n", found);
    if(found == -1) //not found
        return 0;

    //print current data
    for(int i = 0; i < 5; i++)
        printf("%.2X ", data[found + i]);
    puts("");

    //search & replace
    if(!patternsnr(data, sizeof(data), "68 ?? ?1 0? 00", "?? ?1 1? 21 23"))
        return 0; //search & replace failed

    //print replaced data
    for(int i = 0; i < 5; i++)
        printf("%.2X ", data[found + i]);
    puts("");

    return 0;
}
