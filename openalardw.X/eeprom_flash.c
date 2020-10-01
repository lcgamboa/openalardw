/* ########################################################################

   Openalardw - Open Firmware para central de alarme Alard Max WiFi
   https://github.com/lcgamboa/openalardw

   ########################################################################

   Copyright (c) : 2020  Luis Claudio Gambôa Lopes

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */

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

