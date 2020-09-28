#include "config.h"
#include <xc.h>
#include "eeprom_flash.h"

static void Write(unsigned char cfg) {
    unsigned char gie = INTCONbits.GIE;

    INTCONbits.GIE = 0;

    EECON1 = cfg;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1; //;WRITE

    INTCONbits.GIE = gie;
}

void flash_erase_block(unsigned long addr) {


    TBLPTRU = (addr & 0x00FF0000L) >> 16;
    TBLPTRH = (addr & 0x0000FF00L) >> 8;
    TBLPTRL = (addr & 0x000000FFL);

    Write(0b00010100); //Setup erase

    EECON1bits.WREN = 0; //;disable writes
}

void flash_write_block(unsigned long addr, unsigned char * buffer) {
    unsigned char counter;

    TBLPTRU = (addr & 0x00FF0000L) >> 16;
    TBLPTRH = (addr & 0x0000FF00L) >> 8;
    TBLPTRL = (addr & 0x000000FFL);

    asm("TBLRD*-");

    counter = 64;
    do {
        TABLAT = *buffer; // put 1 byte
        buffer++;
        asm("tblwt+*");
        counter--;
    } while (counter);

    Write(0b00000100); //Setup writes block

    EECON1bits.WREN = 0; //;disable writes
}


//addr must be even 

void flash_write_word(unsigned long addr, unsigned short val) {

    TBLPTRU = (addr & 0x00FF0000L) >> 16;
    TBLPTRH = (addr & 0x0000FF00L) >> 8;
    TBLPTRL = (addr & 0x000000FFL);

    TABLAT = (val & 0x00FF); // put 1 byte
    asm("tblwt*+");
    TABLAT = (val & 0xFF00) >> 8; // put 1 byte
    asm("tblwt*");


    Write(0b00100100); //Setup writes word

    EECON1bits.WREN = 0; //;disable writes
}

void flash_write_dword(unsigned long addr, unsigned long val) {
    flash_write_word(addr, val & 0x0000FFFFL);
    flash_write_word(addr + 2, (val & 0xFFFF0000L) >> 16);
}

