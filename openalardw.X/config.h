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

#ifndef CONFIG_H
#define	CONFIG_H

//definicoes de valores de tempo de funcionamento
#define TOUT   3000    //timeout das requisicoes seriais em ms, 0 = sem timeout

#define TSIRENE 180  //tempo de acionamento da sirente em segundos
#define TKEEPAL 300  //tempo de sinal de vida, usar sempre maior que o tempo de sirene

//#define DESLIGAR_SFIO  //se definido desliga entrada de sensores de fio

//definicoes gerais - nao modificar

#define NOWIFI  0
#define WIFIOK  1
#define WIFICFG 2


#define AL_CONFIG     0x01
#define AL_CADASTRO   0x02
#define AL_DISP_ON    0x04
#define AL_DISP_OFF   0x08
#define AL_WIFI_ON    0x10
#define AL_WIFI_OFF   0x20
#define AL_KEEP_AL    0x40
#define AL_SETOR_ON   0x80

#define REMOTE_SENSOR  1
#define REMOTE_CMD_ON  2
#define REMOTE_CMD_OFF 3

typedef union{
unsigned char data[1024];//page size    
struct{
char ID[64]; //identificador da central      
char SERVER[64]; //endereco servidor para envio de status
char PORT[64]; //porta servidor para envio de status
char TOKEN[256]; //TOKEN de acesso do home assistant
};
}config_t;


#endif	

