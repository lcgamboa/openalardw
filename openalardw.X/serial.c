/* ########################################################################

   PICsim - PIC simulator http://sourceforge.net/projects/picsim/

   ########################################################################

   Copyright (c) : 2014-2017  Luis Claudio Gambôa Lopes

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

#include"serial.h"
#include"config.h"
#include"hal.h" 


void serial_init(void) {

    //((_XTAL_FREQ/(4l*56700))-1)
    SPBRG = 211; //56700 em 8MHz
    SPBRGH = 0;
    TXSTAbits.BRGH = 1; //high baud rate
    BAUDCON1bits.BRG16 = 1;

    //Configuracao da serial
    TXSTAbits.TX9 = 0; //transmissao em 8 bits
    TXSTAbits.TXEN = 1; //habilita transmissao
    TXSTAbits.SYNC = 0; //modo assincrono
    RCSTAbits.SPEN = 1; //habilita porta serial - rx
    RCSTAbits.RX9 = 0; //recepcao em 8 bits
    RCSTAbits.CREN = 1; //recepcao continua

#ifdef USE_SERIAL_RX_INT
    PIE1bits.RCIE = 1;
#endif    

}

void serial_tx(unsigned char val) {
    TXREG = val;
    while (!TXSTAbits.TRMT);
}

void serial_tx_str(const char* val) {
    unsigned int i = 0;

    while (val[i]) {
        serial_tx(val[i]);
        i++;
    }
}

/*
unsigned char serial_rx(unsigned int timeout) {
    unsigned int to = 0;

    if (RCSTAbits.FERR || RCSTAbits.OERR)//trata erro
    {
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }

#ifdef USE_SERIAL_RX_INT    
    char val=0x5;
    
    while (((to < timeout) || (!timeout))&&(!serial_get_from_buffer(&val))) {
        delay_ms(1);
        to++;
    }
    return val;
#else    
    while (((to < timeout) || (!timeout))&&(!PIR1bits.RCIF)) {
        delay_ms(1);
        to++;
    }
    if (PIR1bits.RCIF)
        return RCREG;
    else
        return 0x5;
#endif    
}


char* serial_rx_str(char * buff, unsigned int size, unsigned int timeout) {
    unsigned int to = 0;
    unsigned int i;
    if (RCSTAbits.FERR || RCSTAbits.OERR)//trata erro
    {
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }

    size--;
    for (i = 0; i < size;) {
        
#ifdef USE_SERIAL_RX_INT   
    buff[i]=0x5;    
    while (((to < timeout) || (!timeout))&&(!serial_get_from_buffer(&buff[i]))) {
        delay_ms(1);
        to++;
    }
    if(buff[i] == 0x5) //timeout
    {
        buff[i] = 0; //terminador NULL 
        return buff;
    }
#else        
        while (((to < timeout) || (!timeout))&&(!PIR1bits.RCIF)) {
            delay_ms(1);
            to += 1;
        }
        if (PIR1bits.RCIF) {
            do {
                buff[i] = RCREG;
                i++;
            } while ((PIR1bits.RCIF)&&(i < size));
        } else {
            buff[i] = 0; //terminador NULL 
            return buff;
        }
#endif        
    }
    buff[i] = 0; //final buffer 
    return buff;
}
 */

char* serial_rx_str_until(char * buff, unsigned int size, unsigned char term, unsigned int timeout) {
    unsigned int to = 0;
    unsigned int i;

    if (RCSTAbits.FERR || RCSTAbits.OERR)//trata erro
    {
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }

    size--;
    i = 0;
    do {
#ifdef USE_SERIAL_RX_INT   
        buff[i] = 0x5;
        while (((to < timeout) || (!timeout))&&(!serial_get_from_buffer(&buff[i]))) {
            delay_ms(1);
            to++;
        }
        if (buff[i] == 0x5) //timeout
        {
            buff[i] = 0; //terminador NULL 
            return buff;
        } else {
            i++;
        }
#else        
        while (((to < timeout) || (!timeout))&&(!PIR1bits.RCIF)) {
            delay_ms(1);
            to += 1;
        }
        if (PIR1bits.RCIF) {
            do {
                buff[i] = RCREG;
                i++;
            } while ((PIR1bits.RCIF)&&(i < size)&&(buff[i - 1] != term));
        }
#endif    
    } while ((i < size)&&(buff[i - 1] != term) && ((to < timeout) || (!timeout)));


    buff[i] = 0; //final buffer 
    return buff;
}

#ifdef USE_SERIAL_RX_INT


#define SBMAX 1024

#define caddr(X) ((X < SBMAX) ? X : X-SBMAX )  



static char sbuff[SBMAX];
static int scount = 0;
static int spointer = 0;

void serial_clear_buffer(void) {
    scount = 0;
}

unsigned char serial_get_from_buffer(char * val) {
    if (scount > 0) {
        *val = sbuff[spointer];
        spointer++;
        scount--;
        if (spointer >= SBMAX)spointer -= SBMAX;
        return 1;
    } else {
        return 0;
    }

}

void serial_handler(void) {
    if (scount < SBMAX) {
        sbuff[caddr(spointer + scount)] = RCREG;
        scount++;
    } else {
        volatile char a = RCREG; //discard  if buffer is full
    }
}
/*
unsigned int serial_avaliable(void)
{
    return scount;
}
 */
#endif
