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

#ifndef PINOS_H
#define	PINOS_H

//definição dos pinos 

//#define DTMF    PORTAbits.RA0  
#define S1        PORTAbits.RA1 
#define S2        PORTAbits.RA2 
#define S3        PORTAbits.RA3
//#define NC      PORTAbits.RA4 
#define S4        PORTAbits.RA5
#define LED1      LATAbits.LATA6
#define RFDATA    PORTAbits.RA7

#define AD        PORTBbits.RB0
#define PGM1      LATBbits.LATB1
#define LINE_CON  LATBbits.LATB2
#define SIRENE    LATBbits.LATB3
#define BUZZER    LATBbits.LATB4
//#define DTMF    PORTBbits.RB5
//#define PGC     PORTBbits.RB6
//#define PGD     PORTBbits.RB7

#define CTS       LATCbits.LATC0 //OUT 
#define RTS       PORTCbits.RC1 //IN
#define WRESET    LATCbits.LATC2 //OUT
//#define NC      PORTCbits.RC3
//#define NC      PORTCbits.RC4
//#define NC      PORTCbits.RC5
#define WRX       LATCbits.LATC6 //OUT 
#define WTX       PORTCbits.RC7 //IN


#define LED2      LATDbits.LATD0
#define LED3      LATDbits.LATD1
#define LED4      LATDbits.LATD2
#define LED5      LATDbits.LATD3
#define LED6      LATDbits.LATD4
#define LED7      LATDbits.LATD5
#define LED8      LATDbits.LATD6
#define LED9      LATDbits.LATD7

#define PGM2      LATEbits.LATE0
#define PGM3      LATEbits.LATE1
#define BOTAO     PORTEbits.RE2


#endif	/* PINOS_H */

