// base sketch from https://brainy-bits.com/tutorials/4-bits-7-segment-led-display-with-arduino/
// changed TM1637 library by niq_ro for degree, r & h letter
// chronometer on TM1637 LED display by niq_ro from http://www.tehnic.go.ro
// & http://arduinotehniq.com/
// version 1j1c1 - 28.04.2016, Craiova
// power control: bottom, upper or both resistors
// time control from 1minute to 99 minutes

#include "TM1637.h"
 
//{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
//0~9,A,b,C,d,E,F,"-"," ",degree,r,h


#define CLK 9//Pins for TM1637       
#define DIO 8
TM1637 tm1637(CLK,DIO);

#include <Encoder.h> // from http://www.pjrc.com/teensy/td_libs_Encoder.html
Encoder knob(2, 3); //encoder connected to pins 2 and 3 (and ground)
#define apas 10      // push button for encoder

#define unu 5  // first stept of power
#define doi 6  // second step of power
#define goarna 7 // buzzer

// These variables are for the push button routine
int buttonstate = 0; //flag to see if the button has been pressed, used internal on the subroutine only
int pushlengthset = 700; // value for a long push in mS
int pushlength = pushlengthset; // set default pushlength
int pushstart = 0;// sets default push value for the button going low
int pushstop = 0;// sets the default value for when the button goes back high

int knobval; // value for the rotation of the knob
boolean buttonflag = false; // default value for the button flag

int settemp = 1;   // power step
int settemp1 = 1;  // minutes
int settemp2 = 0;  // seconds
int settemp3 = 1;  // hybrid
int settemp3a =10; // hybrid

int tpmax = 99;    // maximum tine in minutes
byte intarziere = 100;
 
void setup(){
  
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL); 
  //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
 
  pinMode(apas,INPUT);//push button on encoder connected 
  digitalWrite(apas,HIGH); //Pull at 1 high

  pinMode(unu, OUTPUT);
  pinMode(doi, OUTPUT);
  digitalWrite(unu,LOW); 
  digitalWrite(doi,LOW); 
 
   pinMode(goarna, OUTPUT);   
   digitalWrite(goarna, LOW); //set in 0 logical level
   tone(goarna, 4000,50); 
   noTone(goarna);
 
    tm1637.display(3,16);    // - first digit from right
    tm1637.display(2,17);    // -
    tm1637.display(1,16);    // -
    tm1637.display(0,17);    // - first digit from left
delay(750);//Delay to let system boot
    tm1637.display(3,17);    // - first digit from right
    tm1637.display(2,16);    // -
    tm1637.display(1,17);    // -
    tm1637.display(0,16);    // - first digit from left
delay(750);
 
 
}//end "setup()"
 
 
 
void loop(){
  //Start of Program 

    knob.write(0);
    delay (100);
    
    knobval=knob.read();
    if (knobval < -1) { //bit of software de-bounce
      knobval = -1;
    }
    if (knobval > 1) {
      knobval = 1;
    }
    settemp=settemp + knobval;
    if (settemp < 1) { //Year can't be older than currently, it's not a time machine.
      settemp = 1;
    }
    if (settemp > 3) { //Year can't be older than currently, it's not a time machine.
      settemp = 3;
    }
tm1637.point(POINT_OFF);
if ((settemp == 1) || (settemp == 2)) {
    tm1637.display(3,settemp);    // clear first digit from right
    tm1637.display(2,17);    // clear
    tm1637.display(1,17);    // clear
    tm1637.display(0,17);    // clear first digit from left
}
if (settemp == 3 ) {
    tm1637.display(3,2);    // no.2
    tm1637.display(2,16);   // -
    tm1637.display(1,1);    // no.1
    tm1637.display(0,17);    // clear first digit from left
}

    pushlength = pushlengthset;
    pushlength = getpushlength ();
    delay (10);

   
    if (pushlength > pushlengthset) {    
//   LongPush();  
    tm1637.display(3,16);    // - first digit from right
    tm1637.display(2,17);    // -
    tm1637.display(1,16);    // -
    tm1637.display(0,17);    // - first digit from left
delay(750);//Delay to let system boot
    tm1637.display(3,17);    // - first digit from right
    tm1637.display(2,16);    // -
    tm1637.display(1,17);    // -
    tm1637.display(0,16);    // - first digit from left
delay(750);
 
delay(150);
    }
    
       
       //This runs the setclock routine if the knob is pushed for a long time
       if (pushlength < pushlengthset) {
              settimp();
              munca();  
              
         pushlength = pushlengthset;
       };


delay(intarziere);
    tm1637.display(3,17);    // clear first digit from right
    tm1637.display(2,17);    // clear
    tm1637.display(1,17);    // clear
    tm1637.display(0,17);    // clear first digit from left
delay(intarziere);


}// end loop() 


// subroutine to return the length of the button push.
int getpushlength () {
  buttonstate = digitalRead(apas);  
       if(buttonstate == LOW && buttonflag==false) {     
              pushstart = millis();
              buttonflag = true;
          };
          
       if (buttonstate == HIGH && buttonflag==true) {
         pushstop = millis ();
         pushlength = pushstop - pushstart;
         buttonflag = false;
       };
       return pushlength;
}

void munca () {
    tone(goarna, 4000,50);
    noTone(goarna);
if (settemp == 1) {
  digitalWrite(unu, HIGH);
  digitalWrite(doi, LOW);
}
if (settemp == 2) {
  digitalWrite(unu, LOW);
  digitalWrite(doi, HIGH);
}
if (settemp == 3) {
  digitalWrite(unu, HIGH);
  digitalWrite(doi, HIGH);
}


//int timp = 60*settemp1 + settemp2;
int timp = settemp3*60;

for (int contor = timp; contor >= 0 ; contor --) {

    pushlength = pushlengthset;
    pushlength = getpushlength ();
    delay (10);

byte minuti2 = contor/60;
// byte secunde2 = contor - 60*contor; //linie gresita
byte secunde2 = contor - 60*minuti2;

byte zmin2 = minuti2/10;
byte umin2 = minuti2 - 10*zmin2;
byte zsec2 = secunde2/10;
byte usec2 = secunde2 - 10*zsec2;

    tm1637.display(3,usec2);    // 
    tm1637.display(2,zsec2);    // 
    tm1637.display(1,umin2);    // 
    tm1637.display(0,zmin2);    // 
    tm1637.point(POINT_ON);
delay(500);  
//    tm1637.point(POINT_OFF);
delay(500);      

   
    if (pushlength > pushlengthset) {    
contor = 0;  
 digitalWrite(unu, LOW);
 digitalWrite(doi, LOW);
tone(goarna, 3000,200);
tone(goarna, 4000,750);
     
tm1637.point(POINT_OFF);
tm1637.display(0,17); // 
tm1637.display(1,14); // E
tm1637.display(2,19); // r
tm1637.display(3,19); // r
  
delay(1000);   
    }
  

    if (contor < 6){
  //     digitalWrite(goarna, HIGH);
       tone(goarna, 3000,200);
    if (contor < 2) {
      tone(goarna, 4000,750);
    }
    }  

}    
/*
    tm1637.display(3,0);    // 0
    tm1637.display(2,0);    // 0
    tm1637.display(1,0);    // 0
    tm1637.display(0,0);    // 0
*/   
 digitalWrite(unu, LOW);
 digitalWrite(doi, LOW);
//delay(1000);    
/*  
    tm1637.display(0,17);    // 0 first digit from right
    tm1637.display(1,17);    // 0
    tm1637.display(2,17);    // 0
    tm1637.display(3,17);    // 0 first digit from left
delay(100);
    tm1637.display(0,16);    // clear first digit from right
    tm1637.display(1,16);    // clear
    tm1637.display(2,16);    // clear
    tm1637.display(3,16);    // clear first digit from left
*/
tm1637.point(POINT_OFF);
for (int i = 3; i > 0 ; i --) {
/*
    tm1637.display(3,0); // units for seconds
    tm1637.display(2,0); // decimal for seconds
    tm1637.display(1,0); // units for minutes
    tm1637.display(0,0); // decimal value for minutes
*/
    tm1637.display(0,17);    // empty
    tm1637.display(1,14);    // E
    tm1637.display(2,21);    // n
    tm1637.display(3,13);    // d

delay(500);
    tm1637.display(0,17);    // empty
    tm1637.display(1,17);    // empty
    tm1637.display(2,17);    // empty
    tm1637.display(3,17);    // empty
delay(500);
}


}

//sets the parameters
void settimp (){
   tone(goarna, 2000,50);
   noTone(goarna);
combinat();
 
   delay (1000);    
}


int combinat() {
    pushlength = pushlengthset;
    pushlength = getpushlength ();
    if (pushlength != pushlengthset) {
      return settemp3;
    }

    knob.write(0);
    delay (100);
    
    knobval=knob.read();
    if (knobval < -1) { //bit of software de-bounce
      knobval = -1;
    }
    if (knobval > 1) {
      knobval = 1;
    }
    settemp3 = settemp3 + knobval;
    if (settemp3 < 1) { //Year can't be older than currently, it's not a time machine.
      settemp3 = tpmax;
    }
    if (settemp3 > tpmax) { //Year can't be older than currently, it's not a time machine.
      settemp3 = 1;
    }

settemp3a = settemp3 *60;     
byte minuti3 = settemp3a/60;
byte secunde3 = settemp3a - 60*minuti3;

byte zmin3 = minuti3/10;
byte umin3 = minuti3 - 10*zmin3;
byte zsec3 = secunde3/10;
byte usec3 = secunde3 - 10*zsec3;
       
tm1637.point(POINT_ON);    
    tm1637.display(3,usec3); // units for seconds
    tm1637.display(2,zsec3); // decimal for seconds
    tm1637.display(1,umin3);  // units for minutes
    tm1637.display(0,zmin3);  // decimal value for minutes
delay(intarziere);
    tm1637.display(3,17); // units for seconds
    tm1637.display(2,17); // decimal for seconds
    tm1637.display(1,17); // units for minutes
    tm1637.display(0,17); // decimal value for minutes
delay(intarziere);
 
    combinat();
}



  
