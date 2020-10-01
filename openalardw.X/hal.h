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

#ifndef HAL_H
#define	HAL_H

#include <xc.h>
#include "pinos.h"

#define _XTAL_FREQ 48000000L

#define EscrevePino(pino,valor)   pino=valor
#define LePino(pino)    pino

#define delay_us(X)  __delay_us(X) 
#define delay_ms(X)  __delay_ms(X)


void hal_inicializa_hw(void);
void hal_wdt(unsigned char on);
void hal_int(unsigned char on);
void hal_InitTimer0(void);
void hal_InitTimer2(void);

#define inicia_tempo() msegundos=0;segundos=0;

extern unsigned int msegundos;
extern unsigned int segundos;


//memoria flash utilizada para guardar configurações
extern const config_t eeprom_cfg; //configuração dos dados do servidor
extern const unsigned long eeprom_sen[256]; //configuração dos sensores
extern const unsigned long eeprom_ctr[256]; //configuração dos controles


#endif	/* HAL_H */

