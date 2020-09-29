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

