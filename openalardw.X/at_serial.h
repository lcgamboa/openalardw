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

#ifndef AT_SERIAL_H
#define	AT_SERIAL_H

extern unsigned long remote;

unsigned char SendAT(const char * cmd, int timeout); 

unsigned char WaitWIND(unsigned char num, int timeout);

unsigned char FindRemote(int timeout);

unsigned char FindConfig(int timeout);

unsigned char ServerStatus(void);

unsigned char SendToServer(const unsigned char  cmd, const char * code); 

#endif	/* AT_SERIAL_H */

