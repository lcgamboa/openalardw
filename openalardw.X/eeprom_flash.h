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

#ifndef EEPROM_FLASH_H
#define	EEPROM_FLASH_H

void flash_erase_block(unsigned long addr); //erase 1024 block
void flash_write_block(unsigned long addr, unsigned char * buffer); //write 64 bytes block
void flash_write_word(unsigned long addr, unsigned short val); //write one word (2 bytes) value
void flash_write_dword(unsigned long addr, unsigned long val); //write one dword (4 bytes) value

#endif	/* EEPROM_FLASH_H */

