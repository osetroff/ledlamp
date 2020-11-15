//turn on/off 5pcs led lamps
//atmega328p 8mhz int dip28
/*
 * 5 input pins
 * 
 * pin - 330 - switch - gnd
 * pin - 20k - VCC
*/
/*
 * 5 output pins
 * 
 * pin - 20k - GND
 * pin - solid state rele 5v
 */

#include "k_atmega328p_8mhz.cpp"

//-----------------
//led 
//PB0
#define led_pin _0
inline static void led_pin_high(void){pbh(led_pin);}
inline static void led_pin_low(void){pbl(led_pin);}
inline static void led_pin_xor(void){pbx(led_pin);}
inline static void initLedPin(void){pbo(led_pin);pbl(led_pin);}

//-----------------
//output pins
//PB1..PB5
#define OutPinsMask (_1|_2|_3|_4|_5)

static void initOutPins(){
    //turn to low
    pbl(OutPinsMask);
    //set to output
    pbo(OutPinsMask);
}

//-----------------
//input pins
//PD3..PD7
#define InPinsMask (_3|_4|_5|_6|_7)

static void initInPins(){
    //set to input
    pdi(InPinsMask);
}

inline static void sp(char lc){serial_write(lc);}
#define spn sp(0xd);sp(0xa)
#define spc sp(' ')
inline static void s(const char *ls){serial_sendString(ls);}
//=========================
int main(void){
    atmega328p_init();
    
    initLedPin();
    initOutPins();
    initInPins();
        
//    u16 lpause=200;
    serial_init();
    s("Start1");spn;
    s("Start2");spn;
    s("Start3");spn;
    
    while (1) {
    	
        s("123456789"); spn;
//        lpl;
        //serial_wait(100);
        //dms(100);
        //UCSR0B = 0;
        //dms(10);
        pwrdown(_1s,b0);
        //pwrsave(_1s,b0,t20);
        //UCSR0B = (1 << RXEN0) | (1 << TXEN0);
        dms(10);
        //lpl;

//        lph;
//        serial_wait(100);
//        UCSR0B = 0
//        pwrdown(_1s,b0);
//        power_usart0_enable();
//        dms(lpause);
//        lph;dms(lpause);
//    	lpl;dms(lpause);
    }

}