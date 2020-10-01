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

