/* 
 * File:   eeprom_flash.h
 * Author: gamboa
 *
 * Created on 22 de Setembro de 2020, 23:22
 */

#ifndef EEPROM_FLASH_H
#define	EEPROM_FLASH_H

void flash_erase_block(unsigned long addr); //erase 1024 block
void flash_write_block(unsigned long addr, unsigned char * buffer); //write 64 bytes block
void flash_write_word(unsigned long addr, unsigned short val); //write one word (2 bytes) value
void flash_write_dword(unsigned long addr, unsigned long val); //write one dword (4 bytes) value

#endif	/* EEPROM_FLASH_H */

