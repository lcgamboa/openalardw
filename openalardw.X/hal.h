/* 
 * File:   hal.h
 * Author: gamboa
 *
 * Created on 27 de Setembro de 2020, 10:50
 */

#ifndef HAL_H
#define	HAL_H

#include <xc.h>
#include "pinos.h"

#define _XTAL_FREQ 24000000L

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

