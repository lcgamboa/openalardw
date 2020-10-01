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

char * utoa(unsigned int val, char* str )
{
  str[0]=(val/10000)+0x30;  
  str[1]=((val%10000)/1000)+0x30;  
  str[2]=((val%1000)/100)+0x30;  
  str[3]=((val%100)/10)+0x30;
  str[4]=(val%10)+0x30;
  str[5]=0;

  return str;
}

char * utoa2(unsigned int val, char* str )
{
    unsigned char  i;
    
    utoa(val, str);
    
    while(str[0]=='0')
    {
       i=0; 
       while(str[i+1] != 0 )
       {
           str[i]=str[i+1];
           i++;
       }
       str[i]=0;
    }
    return str;
}

/*
char * itoa( int val, char* str )
{
    if(val >= 0)
    {
       utoa((unsigned int)val,str+1);
       str[0]='+';
    }
    else
    {
       utoa(((unsigned int)(-val)),str+1);
       str[0]='-';
    }
    
    return str;
}



char * ultoa(unsigned long val, char* str )
{
  str[0]=((val%10000000000L)/1000000000L)+0x30;     
  str[1]=((val%1000000000L) /100000000L)+0x30;  
  str[2]=((val%100000000L)  /10000000L)+0x30;  
  str[3]=((val%10000000L)   /1000000L)+0x30;    
  str[4]=((val%1000000L)    /100000L)+0x30;  
  str[5]=((val%100000L)     /10000)+0x30;  
  str[6]=((val%10000)      /1000)+0x30;  
  str[7]=((val%1000)       /100)+0x30;  
  str[8]=((val%100)        /10)+0x30;
  str[9]=(val%10)         +0x30;
  str[10]=0;

  return str;
}
*/

char * ltoha(unsigned long val, char* str )
{
    int i;
    
    str[0]='0';
    str[1]='x';
    str[2]= (val & 0xF0000000L)>>28;
    str[3]= (val & 0x0F000000L)>>24;
    str[4]= (val & 0x00F00000L)>>20;
    str[5]= (val & 0x000F0000L)>>16;
    str[6]= (val & 0x0000F000L)>>12;
    str[7]= (val & 0x00000F00L)>>8;
    str[8]= (val & 0x000000F0L)>>4;
    str[9]= (val & 0x0000000FL);    
    str[10]=0;
    
    for(i=2;i<10;i++)
    {
        if(str[i] < 10)
        {
            str[i]+='0';
        }
        else
        {
            str[i]+='A'-10;
        }
    }
  return str;
}
