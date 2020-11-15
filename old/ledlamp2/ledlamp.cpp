#include <avr/wdt.h>



#define _0 (1<<0)
#define _1 (1<<1)
#define _2 (1<<2)
#define _3 (1<<3)
#define _4 (1<<4)
#define _5 (1<<5)
#define _6 (1<<6)
#define _7 (1<<7)
//output
#define pbo(m) DDRB |=(m)
#define pco(m) DDRC |=(m)
#define pdo(m) DDRD |=(m)
//input
#define pbi(m) DDRB &=~(m)
#define pci(m) DDRC &=~(m)
#define pdi(m) DDRD &=~(m)
//high
#define pbh(m) PORTB |=(m)
#define pch(m) PORTC |=(m)
#define pdh(m) PORTD |=(m)
//low
#define pbl(m) PORTB &=~(m)
#define pcl(m) PORTC &=~(m)
#define pdl(m) PORTD &=~(m)
//xor
#define pbx(m) PORTB ^=(m)
#define pcx(m) PORTC ^=(m)
#define pdx(m) PORTD ^=(m)

//==Delay
void dus(uint16_t tic_us){
	tic_us <<= 1; //1us = 4 öèêëà
	__asm__ volatile (
			"1: sbiw %0,1" "\n\t"
			"brne 1b"
			: "=w" (tic_us)
			: "0" (tic_us)
		  );
}
void dms(uint16_t tic_ms){
  //correction
  uint16_t tic_us=(tic_ms + (tic_ms>> 2)+(tic_ms>> 11))<<1;
  if (tic_us>0) {
  __asm__ volatile (
    "1: sbiw %0,1" "\n\t"
    "brne 1b"
    : "=w" (tic_us)
    : "0" (tic_us)
   );
  }
  while(tic_ms-->0){
    tic_us=1000 << 1;
    __asm__ volatile (
			"1: sbiw %0,1" "\n\t"
			"brne 1b"
			: "=w" (tic_us)
			: "0" (tic_us)
		 );
  }
}





int main(void){
	wdt_disable();
	uint8_t pin=_0;
    uint16_t lpause=5000;
	pbo(pin);
    while (1) {
    	pbh(pin);
    	dms(lpause);
    	pbl(pin);
    	dms(lpause);
    }

}