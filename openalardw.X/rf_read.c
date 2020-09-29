
/*
 based on code:
 http://acturcato.wordpress.com/2014/01/04/decoder-for-ht6p20b-encoder-on-arduino-board-english/
 */

#include <xc.h>
#include "rf_read.h"



unsigned int pulseIn(char state)
{
    unsigned int delay=0;

     TMR0H=0;
     TMR0L=0;
     INTCONbits.TMR0IF = 0;
     
     while((pinRF_RX == state)&&(!INTCONbits.TMR0IF));
     
     if(INTCONbits.TMR0IF) return 0;
     TMR0H=0;
     TMR0L=0;
     INTCONbits.TMR0IF = 0;
     
     while((pinRF_RX == !state)&&(!INTCONbits.TMR0IF));
     
     if(INTCONbits.TMR0IF) return 0;
     TMR0H=0;
     TMR0L=0;
     INTCONbits.TMR0IF = 0;
     
     while((pinRF_RX == state)&&(!INTCONbits.TMR0IF));
    
     if(INTCONbits.TMR0IF) return 0;
     delay=TMR0L;
     delay|=TMR0H<<8;
     
     delay=(delay<<1)/3; //ajuste 48MHz
     
    return  delay ;
}

#define bitRead(x, bit)  ((x & (1<<bit))>0)

unsigned char receiveAllCodes(rfControl *_rfControl)
{ 
  static unsigned char  startbit=0;      //checks if start bit was identified
  static int counter=0;           //received bits counter: 22 of Address + 2 of Data + 4 of EndCode (Anti-Code)
  static unsigned long buffer=0;  //buffer for received data storage
  
  static int hlambda;      // half on pulse clock width (if fosc = 2KHz than hlambda = 250 us)
  
  int dur0, dur1;  // pulses durations (auxiliary)
  int tout=0;
  
  while (!startbit && (tout < 20)) //wait 20 pulses
  {// Check the PILOT CODE until START BIT;
    dur0 = pulseIn(0);  //Check how long DOUT was "0" (ZERO) (refers to PILOT CODE)
        
    //If time at "0" is between 9200 us (23 cycles of 400us) and 13800 us (23 cycles of 600 us).
    if((dur0 > 9200) && (dur0 < 13800) && !startbit)
    {    
      //calculate wave length - lambda
      hlambda = dur0 / 46;
      
      //Reset variables
      dur0 = 0;
      buffer = 0;
      counter = 0;
      
      startbit = 1;
    }
    tout++;
  }

  //If Start Bit is OK, then starts measure os how long the signal is level "1" and check is value is into acceptable range.
  if (startbit && counter < 28)
  {
    ++counter;
    
    dur1 = pulseIn(1);
    
    if((dur1 >  hlambda) && (dur1 < (3 * hlambda)))  //If pulse width at "1" is between "0.5 and 1.5 lambda", means that pulse is only one lambda, so the data é "1".
    {
      buffer = (buffer << 1) + 1;   // add "1" on data buffer
    }
    else if((dur1 > 3 * hlambda) && (dur1 < (5 * hlambda)))  //If pulse width at "1" is between "1.5 and 2.5 lambda", means that pulse is two lambdas, so the data é "0".
    {
      buffer = (buffer << 1);       // add "0" on data buffer
    }
    else
    {
      //Reset the loop
      startbit = 0;
    }
  }
  
  //Check if all 28 bits were received (20 of Address + 4 of Data + 4 of Anti-Code)
  if (counter==28) 
  { 
    // Check if Anti-Code is OK (last 4 bits of buffer equal "0101")
    if ((bitRead(buffer, 0) == 1) && (bitRead(buffer, 1) == 0) && (bitRead(buffer, 2) == 1) && (bitRead(buffer, 3) == 0))
    {     
      counter = 0;
      startbit = 0;
      
      //Get ADDRESS CODE from Buffer
      _rfControl->addr = buffer;// >> 8;
      
      //Get Buttons from Buffer
       _rfControl->btn1 = bitRead(buffer,4);
       _rfControl->btn2 = bitRead(buffer,5);
       _rfControl->btn3 = bitRead(buffer,6);
       _rfControl->btn4 = bitRead(buffer,7);
       
      //If a valid data is received, return OK
      return RF_RECEIVED;
    }
    else
    {
      //Reset the loop
      startbit = 0;
    }
  }
  
  
  if(startbit)
  {
      return RF_RECEIVING;
  }
  
  return RF_IDLE;
}
