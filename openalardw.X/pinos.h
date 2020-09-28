/* 
 * File:   pinos.h
 * Author: gamboa
 *
 * Created on 27 de Setembro de 2020, 10:43
 */

#ifndef PINOS_H
#define	PINOS_H

//definição dos pinos 

//#define DTMF    PORTAbits.RA0  
#define S1        PORTAbits.RA1 
#define S2        PORTAbits.RA2 
#define S3        PORTAbits.RA3
//#define NC      PORTAbits.RA4 
#define S4        PORTAbits.RA5
#define LED1      PORTAbits.RA6
#define RFDATA    PORTAbits.RA7

#define AD        PORTBbits.RB0
#define PGM1      PORTBbits.RB1
#define LINE_CON  PORTBbits.RB2
#define SIRENE    PORTBbits.RB3
#define BUZZER    PORTBbits.RB4
//#define DTMF    PORTBbits.RB5
//#define PGC     PORTBbits.RB6
//#define PGD     PORTBbits.RB7

#define CTS       PORTCbits.RC0 //OUT 
#define RTS       PORTCbits.RC1 //IN
#define WRESET    PORTCbits.RC2 //OUT
//#define NC      PORTCbits.RC3
//#define NC      PORTCbits.RC4
//#define NC      PORTCbits.RC5
#define WRX       PORTCbits.RC6 //OUT 
#define WTX       PORTCbits.RC7 //IN


#define LED2      PORTDbits.RD0
#define LED3      PORTDbits.RD1
#define LED4      PORTDbits.RD2
#define LED5      PORTDbits.RD3
#define LED6      PORTDbits.RD4
#define LED7      PORTDbits.RD5
#define LED8      PORTDbits.RD6
#define LED9      PORTDbits.RD7

#define PGM2      PORTEbits.RE0
#define PGM3      PORTEbits.RE1
#define BOTAO     PORTEbits.RE2


#endif	/* PINOS_H */

