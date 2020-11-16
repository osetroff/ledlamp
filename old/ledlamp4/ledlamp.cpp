//turn on/off 5pcs led lamps
//atmega328p 8mhz int dip28
/*
 * 5 input pins
 * 
 * pin - 330 - switch - gnd
 * pin - 20k - VCC
 
  Bits:
      3   4  GND  5   6  GND  7  GND 
      |   |   |   |   |   |   |   |
*/
/*
 * 5 output pins
 * 
 * pin - 20k - GND
 * pin - solid state rele 5v
 */

#include "k_atmega328p_8mhz.cpp"

//-----------------
// led 
//PB0
#define led_pin _0
inline static void led_pin_high(void){pbh(led_pin);}
inline static void led_pin_low(void){pbl(led_pin);}
inline static void led_pin_xor(void){pbx(led_pin);}
inline static void initLedPin(void){pbo(led_pin);pbl(led_pin);}

//-----------------
// input pins
//PD3..PD7
#define InPinsMask (_3|_4|_5|_6|_7)
//read InPins 
inline volatile static u8 readInPins() {
    return (PIND&InPinsMask);
}

static void initInPins(){
    //set to input
    pdi(InPinsMask);
}

//-----------------
// output pins
//PB1..PB5
#define OutPinsMask (_1|_2|_3|_4|_5)
//read InPins 
volatile static void setOutPins(u8 pins) {
    PORTB=((PORTB&(~(OutPinsMask)))|pins);
}            

static void initOutPins(){
    //turn to low
    pbl(OutPinsMask);
    //set to output
    pbo(OutPinsMask);
}

//--------------------
// serial output
inline static void sp(char lc){serial_write(lc);}
#define spn sp(0xd);sp(0xa)
#define spc sp(' ')
inline static void s(const char *ls){serial_sendString(ls);}
//sph hex
static inline void sphc(char a){if (a>9) {a+='A'-10;} else {a+='0';} sp(a);}
static void sph(uint8_t a) {sphc(a>>4);sphc(a&0xF);}
static void sph16(uint16_t a) {sph(a>>8);sph(a&0xFF);}


//------------------
// Lamps
typedef struct {
    u8 inPinMask;
    u8 outPinMask;
    u8 isOn;//=0 if lamp is off
    u8 timePause;//time for pause between lamp can be on or off
    u8 timePauseDef;//default time for pause
} Lamp_t;
//array of lamp
static Lamp_t Lamps[]={
    //?
    {0,0,0,0,3},
    //hall 1
    {0,0,0,0,3},
    //cabinet
    {0,0,0,0,3},
    //bedroom
    {0,0,0,0,3},
    //hall 2
    {0,0,0,0,3},
};
//lamps count
#define LampsCount   sizeof(Lamps)

//=========================
int main(void){
    //-------------
    // init
    atmega328p_init();
    initLedPin();
    initOutPins();
    initInPins();
        
    //---------------
    // serial
    serial_init();
    sp(Lamps+'0');s(" lamps.");spn;
    s("Start2");spn;
    s("Start3");spn;
    
    //-----------------
    // save initial InPins
    //means all lamps are off    
    u8 InPins=readInPins();
    //sph(InPins);spn;

    // pause to not allow change switch quickly
    u8 timePause=0;
    
    //=============================
    // main loop
    
    while (1) {
    	
        // get actual InPins
        u8 curInPins=readInPins();
        s("> ");sph(curInPins);spn;
        
        // some switches have been pressed
        if (curInPins!=InPins) {
            
            
            // for each lamp look for switch changes
            for (u8 li=0;li<LampsCount;li++) {
                
                // compare input pin for switch of lamp li
                if ((curInPins&Lamps[li].inPinMask)!=(InPins&Lamps[li].inPinMask)) {
                
                    // switch has changed
                    if (Lamps[li].timePause==0) {
                        // lamp can be switched
                        u8 lampIsOn=Lamps[li].isOn;
                        
                        if (lampIsOn==0) {
                            // lamp was off -> turn it on
                            
                        } else {
                            // lamp was on -> turn it off
                            
                        }
                    }
                    
                    
                }
                
            } //for
            
            // save Pins
            InPins=curInPins;
        } 
        //---------------------------
        // wait serial made all output
        dms(1);
        // then sleep
        pwrdown(_1s,b0);
        
        //-------------
        // spend portion of time
        if (timePause!=0) {
            timePause--;
            //TODO for all lamps
        
        }
        

        //process user input
        if (serial_has_input) {
            sp('>');
            serial_sendArray(serial_buf,serial_buf_i);
            spn;
            serial_buf_clear();
        }
    } //while
    //============================
}