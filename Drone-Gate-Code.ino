#include <LiquidCrystal.h>
#include <Ultrasonic.h>
#include <FastLED.h> //Libraries

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Arduino Pins to LCD
#define LED_PIN 0
#define NUM_LEDS 100
CRGB leds[NUM_LEDS];
uint8_t hue = 0;

#define bt_start  A0
#define bt_stop   A1
#define bt_reset  A2
#define trigPin 11
#define echoPin 12
#define G_led 8 
#define R_led 9 
#define trigPin2 10
#define echoPin2 1
#define buzzer 13

int hh=0, mm=0, ss=0, ms=0; 
bool timerStart = false;
Ultrasonic ultrasonic(trigPin, echoPin); 
Ultrasonic ultrasonic2(trigPin2, echoPin2); 

int motionCount = 0;

void setup() { 

pinMode(bt_start, INPUT_PULLUP);
pinMode(bt_stop,  INPUT_PULLUP);
pinMode(bt_reset, INPUT_PULLUP);

FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
FastLED.clear();


pinMode(R_led,OUTPUT); // declare Red LED as output
pinMode(G_led,OUTPUT); // declare Green LED as output
pinMode(buzzer,OUTPUT); // declare Buzzer as output 

lcd.begin(16, 2); // 
lcd.clear();
lcd.setCursor (0,0);
lcd.print("  Welcome  To  ");
lcd.setCursor (0,1);
lcd.print("   Stopwatch   ");
delay(2000);
lcd.clear();
 

 noInterrupts();         // disable all interrupts
 TCCR1A = 0;             // set entire TCCR1A register to 0  //set timer1 interrupt at 1kHz  // 1 ms
 TCCR1B = 0;             // same for TCCR1B
 TCNT1  = 0;             // set timer count for 1khz increments
 OCR1A = 1999;           // = (16*10^6) / (1000*8) - 1
 //had to use 16 bit timer1 for this bc 1999>255, but could switch to timers 0 or 2 with larger prescaler
 // turn on CTC mode
 TCCR1B |= (1 << WGM12); // Set CS11 bit for 8 prescaler
 TCCR1B |= (1 << CS11);  // enable timer compare interrupt
 TIMSK1 |= (1 << OCIE1A);
 interrupts();           // enable

}

void loop() {

long distance = ultrasonic.read();
//long distance2 = ultrasonic2.read();

//long distanceAvg = (distance + distance2) / 2;
if (distance< 35) { //Avg < 35) {
  
  motionCount++; // increment the motion count
  if (motionCount == 2) { // if the motion count is 2, stop the stopwatch
    timerStart = false; // Stop stopwatch 
  } else {
    timerStart = true; // Start stopwatch 
  }
  delay(100);
}

if(digitalRead (bt_reset) == 0){ 
    motionCount = 0; // Reset motion count
    ms=0, ss=0, mm=0, hh=0; // Reset stopwatch 
    delay(100);
}
lcd.setCursor (0,0);
lcd.print("   Stopwatch   ");

lcd.setCursor (2,1);
lcd.print((hh/10)%10);
lcd.print(hh%10);
lcd.print(":");
lcd.print((mm/10)%10);
lcd.print(mm%10);
lcd.print(":");
lcd.print((ss/10)%10);
lcd.print(ss%10);
lcd.print(":");
lcd.print((ms/100)%10);
lcd.print((ms/10)%10);
lcd.print(ms%10);

if(timerStart==true){
digitalWrite(G_led, HIGH); // Turn LED on.  
digitalWrite(R_led, LOW);  // Turn LED off.

//for (int i=0; i<NUM_LEDS; i++ )
//leds[i] = CRGB(0, 175, 80 );
//FastLED.show();
//for (int i=0; i<NUM_LEDS; i++ )
//{
//leds[i] = CRGB(20*i, 255-2*i, 20*i );

//FastLED.show();

//}
for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CHSV(hue + (i * 10), 255, 255);
  }

  //You can change the pattern speed here
  EVERY_N_MILLISECONDS(15){
    hue++;
  }
  
  FastLED.show();
} 

else{
digitalWrite(G_led, LOW); // Turn LED off.   
digitalWrite(R_led, HIGH);  // Turn LED on. 

for (int i=0; i<NUM_LEDS; i++ )
leds[i] = CRGB(255, 0, 0 );
FastLED.show();
}
  
digitalWrite(buzzer, LOW);
}


ISR(TIMER1_COMPA_vect){   
if(timerStart == true){ms=ms+1;
if(ms>999){ms=0;ss=ss+1;
if(ss>59){ss=0; mm=mm+1;}
if(mm>59){mm=0; hh=hh+1;}
  }  
 }
}