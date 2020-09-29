#include "config.h"
#include "serial.h"
#include <xc.h>
#include "pinos.h"
#include "hal.h"



// PIC18F46J50 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config WDTEN = OFF      // Watchdog Timer (Disabled - Controlled by SWDTEN bit)
#pragma config PLLDIV = 2       // PLL Prescaler Selection bits (2 (8 MHz oscillator input drives PLL directly))
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset (Enabled)
#pragma config XINST = OFF      // Extended Instruction Set (Disabled)

// CONFIG1H
#pragma config CPUDIV = OSC1    // CPU System Clock Postscaler (No CPU system clock divide)
#pragma config CP0 = OFF        // Code Protect (Program memory is not code-protected)

// CONFIG2L
#pragma config OSC = INTOSCPLL     // Oscillator (INTOSC)
#pragma config T1DIG = OFF      // T1OSCEN Enforcement (Secondary Oscillator clock source may not be selected)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator (High-power operation)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = OFF       // Internal External Oscillator Switch Over Mode (Disabled)

// CONFIG2H
#pragma config WDTPS = 8192     // Watchdog Postscaler (1:8192)

// CONFIG3L
#pragma config DSWDTOSC = INTOSCREF// DSWDT Clock Select (DSWDT uses INTRC)
#pragma config RTCOSC = T1OSCREF// RTCC Clock Select (RTCC uses T1OSC/T1CKI)
#pragma config DSBOREN = OFF    // Deep Sleep BOR (Disabled)
#pragma config DSWDTEN = OFF    // Deep Sleep Watchdog Timer (Disabled)
#pragma config DSWDTPS = G2     // Deep Sleep Watchdog Postscaler (1:2,147,483,648 (25.7 days))

// CONFIG3H
#pragma config IOL1WAY = OFF    // IOLOCK One-Way Set Enable bit (The IOLOCK bit (PPSCON<0>) can be set and cleared as needed)
#pragma config MSSP7B_EN = MSK7 // MSSP address masking (7 Bit address masking mode)

// CONFIG4L
#pragma config WPFP = PAGE_63   // Write/Erase Protect Page Start/End Location (Write Protect Program Flash Page 63)
#pragma config WPEND = PAGE_WPFP// Write/Erase Protect Region Select (valid when WPDIS = 0) (Page WPFP<5:0> through Configuration Words erase/write protected)
#pragma config WPCFG = OFF      // Write/Erase Protect Configuration Region (Configuration Words page not erase/write-protected)

// CONFIG4H
#pragma config WPDIS = OFF      // Write Protect Disable bit (WPFP<5:0>/WPEND region ignored)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


unsigned int msegundos = 0;
unsigned int segundos = 0;
extern unsigned char AUX_BUZZER;

//memoria flash utilizada para guardar configurações
const config_t eeprom_cfg __at(0xF000); //configuração dos dados do servidor
const unsigned long eeprom_sen[256] __at(0xF400); //configuração dos sensores
const unsigned long eeprom_ctr[256] __at(0xF800); //configuração dos controles


//interrupção 

void __interrupt() isr(void) {

#ifdef USE_SERIAL_RX_INT   
    if (PIR1bits.RC1IF) {
        //verifica recebimento de dados do modulo wifi pela serial
        serial_handler();
    }
#endif  
    if (PIR1bits.TMR2IF) {

        //contagem de tempo em msegundos e segundos
        msegundos++;
        if (msegundos > 1000) {
            msegundos = 0;
            segundos++;
        }

        //acionamento do Buzzer
        if (AUX_BUZZER) {
            BUZZER ^= 1; //oscila buzzer 1KHz
        } else {
            BUZZER = 0;
        }
        PIR1bits.TMR2IF = 0;
    }
}


void hal_inicializa_hw(void)
{
    //configura o clock interno para 8MHz
    OSCCONbits.IRCF = 7; //8MHz
    OSCTUNEbits.PLLEN = 1;

    ANCON0 = 0xFF; //all digital
    ANCON1 = 0x7F; //all digital

    //configura direção dos pinos
    TRISA = 0xBF;
    TRISB = 0xE1;
    TRISC = 0xBA;
    TRISD = 0x00;
    TRISE = 0xFC;
}

void hal_wdt(unsigned char on)
{
    WDTCONbits.SWDTEN = on;
    CLRWDT();
}

void hal_int(unsigned char on)
{
    INTCONbits.GIE = on;
    INTCONbits.PEIE = on;
}

//inicializa TIMER0 utilizado para marcar tempo em usegundos da recepção RF

void hal_InitTimer0(void) {

    T0CONbits.T0CS = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS = 0x02; // divide por 8
    T0CONbits.T08BIT = 0;
    T0CONbits.TMR0ON = 1;

    //INTCONbits.TMR0IE = 0;
    //INTCONbits.TMR0IF = 0;
}

//inicializa TIMER2 utilizado para marcar tempo em msegundos e segundos

void hal_InitTimer2(void) {
    //Configura TMR2

    T2CONbits.TMR2ON = 0;
    T2CONbits.T2CKPS = 0b11; //divide por 16
    T2CONbits.T2OUTPS = 0b0100; //divide por 5
    TMR2 = 0;
    PR2 = 149; //1ms=(4/FOSC)*16*(24+1)*5
    T2CONbits.TMR2ON = 1;
    PIE1bits.TMR2IE = 1;
    PIR1bits.TMR2IF = 0;
}
