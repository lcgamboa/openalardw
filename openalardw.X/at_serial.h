/* 
 * File:   at_serial.h
 * Author: gamboa
 *
 * Created on 14 de Setembro de 2020, 18:00
 */

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

