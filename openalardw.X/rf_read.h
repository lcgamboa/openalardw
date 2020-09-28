/* 
 * File:   rf_read.h
 * Author: gamboa
 *
 * Created on 12 de Setembro de 2020, 22:45
 */

#ifndef RF_READ_H
#define	RF_READ_H

#include "config.h"

#define  pinRF_RX       PORTAbits.RA7    //Pin where RF Receiver Module is connected. If necessary, change this for your project


typedef struct         //Struct for RF Remote Controls
{
   unsigned long addr;    //ADDRESS CODE
   unsigned char btn1;        //BUTTON 1
   unsigned char btn2;        //BUTTON 2
   unsigned char btn3;        //BUTTON 3
   unsigned char btn4;        //BUTTON 4   
}rfControl;


unsigned char receiveAllCodes(rfControl *_rfControl);


#define RF_IDLE      0
#define RF_RECEIVED  1
#define RF_RECEIVING 2

#endif	/* RF_READ_H */

