//turn on/off 5pcs led lamps
//atmega328p 8mhz int dip28
//20201117
#include "k_atmega328p_8mhz.cpp"

//-----------------
// debug output to serial
//#define debug_
#ifdef debug_
#define l(a) a
#else
#define l(a)
#endif

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
/* 5 input pins
 pin - 330 - switch - gnd
 pin - 20k - VCC
 Bits:
      _3  _4  GND _5  _6  GND _7  GND 
      |   |   |   |   |   |   |   |
 top left
*/
//read InPins 
inline volatile static u8 readInPins() {return (PIND&InPinsMask);}
static void initInPins(){pdi(InPinsMask);}

//-----------------
// output pins
//PB1..PB5
#define OutPinsMask (_1|_2|_3|_4|_5)
/* 5 output pins
  pin - 20k - GND
  pin - solid state rele 5v

  Bits 
 top left
       _2   _4   _5   GND
       .    .    .    .
       .    .    .    .
       _1   _3        GND
  bottom
*/
//read InPins 
volatile static void setOutPinHigh(u8 pinmask) {pbh(pinmask);}
volatile static void setOutPinLow(u8 pinmask) {pbl(pinmask);}
static void initOutPins(){
    //turn to low
    pbl(OutPinsMask);
    //set to output
    pbo(OutPinsMask);
}
inline volatile static u8 readOutPins() {return (PORTB&OutPinsMask);}

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
    //In,Out
    {_3,_1,0,0,3},
    {_4,_2,0,0,3},
    {_5,_3,0,0,3},
    {_6,_4,0,0,3},
    {_7,_5,0,0,3},
};
//lamps count
#define LampsCount   (sizeof(Lamps)/sizeof(Lamp_t))

//--------------------
// serial output
inline static void sp(char lc){serial_write(lc);}
#define spn sp(0xd);sp(0xa)
#define sps sp(' ')
#define sp0 sp('0')
inline static void s(const char *ls){serial_sendString(ls);}
//sph hex
static inline void sphc(char a){if (a>9) {a+='A'-10;} else {a+='0';} sp(a);}
static void sph(uint8_t a) {sphc(a>>4);sphc(a&0xF);}
static void sph16(uint16_t a) {sph(a>>8);sph(a&0xFF);}
//decimal output in l0 places
//l0=1
static void sp8(uint8_t la,uint8_t l0=0){
 uint8_t li;
 uint8_t lz=0;
 if (la>=100) {
     li=la/100;
     sp(0x30+li);
     la-=(li*100);
     lz=1;
 } else {if (l0>2) {sp0;}}
 if (la>=10) {
     li=la/10;
     sp(0x30+li);
     la-=(li*10);
 } else {if ((l0>1)||(lz>0)) {sp0;}}
 sp(0x30+la);
}

static void testh(){
    l(
    sph16(testheap());sps;sph16(testsp());spn;
    );
}
static void testl(){            
    l(
    s("=======");spn;
    u8 li=0;
    do {
        sp8(li);sp(':');
        sph(Lamps[li].inPinMask);sps;
        sph(Lamps[li].outPinMask);sps;
        sp8(Lamps[li].isOn);sps;
        sp8(Lamps[li].timePause);sps;
        sp8(Lamps[li].timePauseDef);spn;

    } while (++li<LampsCount);
    );
}

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
    l(
    serial_init();
    sp8(LampsCount);s(" lamps.");spn;
    //s("Start2");spn;//s("Start3");spn;
    //testh();
    testl();
    );
    //-----------------
    // save initial InPins
    //means all lamps are off    
    u8 InPins=readInPins();
    //sph(InPins);spn;

    // pause to not allow change switch quickly
    u8 timePause=0;
    
    
    
    //=============================
    // main loop
    volatile u8 li;
    
    while (1) {
    	
        // get actual InPins
        u8 curInPins=readInPins();
        l(
        s("In:");sph(curInPins);sps;
        s("Out:");sph(readOutPins());spn;
        );
        // some switches have been pressed
        if (curInPins!=InPins) {
            testl();
            
            // for each lamp look for switch changes
            li=0;
            do {
                l(sp8(li);spn;);
                // compare input pin for switch of lamp li
                if ((curInPins&Lamps[li].inPinMask)!=(InPins&Lamps[li].inPinMask)) {
                
                    // switch has changed
                    if (Lamps[li].timePause==0) {

                        l(s("Lamp ");sp8(li););
                        // lamp can be switched
                        if (Lamps[li].isOn==0) {
                            // lamp was off -> turn it on
                            setOutPinHigh(Lamps[li].outPinMask);
                            Lamps[li].isOn=1;
                            l(s(" is on"););
                        } else {
                            // lamp was on -> turn it off
                            setOutPinLow(Lamps[li].outPinMask);
                            Lamps[li].isOn=0;
                            l(s(" is off"););
                        }
                        l(spn;);
                        
                        lph;
                        // set pause
                        Lamps[li].timePause=Lamps[li].timePauseDef;
                        // update timePause if needed
                        if (Lamps[li].timePause>timePause) {timePause=Lamps[li].timePause;}
                        
                    } else {
                        l(s("Skipped");spn;);
                    }
                }
            } while (++li<LampsCount);
            
            testl();
            // save Pins
            InPins=curInPins;
        }
        
        
        //testh();
        //---------------------------
        // wait serial made all output
        dms(1);
        // then sleep
        pwrdown(_1s,b0);
        //serial_init();
        
        
        //testh();
        //-------------
        // decrease timePause
        if (timePause!=0) {
            timePause--;
          
            l(s("timePause:");sp8(timePause);spn;);
          
            u8 lpause;
            // for all lamps
            li=0;
            do {
                lpause=Lamps[li].timePause;
                // 
                if (lpause!=0) {Lamps[li].timePause=lpause-1;}
            } while (++li<LampsCount);
            
            
            if (timePause==0) {lpl;}
            testl();
            
        }
        

        //TODO doesnot work after pwrdown
        // process user input
        l(
        if (serial_has_input) {
            //lph;
            sp('>');
            serial_sendArray(serial_buf,serial_buf_i);
            spn;
            serial_buf_clear();
            dms(30000);
        }
        );
    } //while
    //============================
}