#include <Wire.h>
#include "RTClib.h"
#include <FastLED.h>

RTC_DS3231 rtc;

#define LED_PIN    2 //The data pin of the arduino
#define NUM_LEDS    114 //Numbers of LED
#define LED_TYPE    WS2812B //The type of the LED stripe
#define COLOR_ORDER GRB

#define P1 8 // Button Hours
#define P2 7 // Button Minute
#define A0 0 // Pin for Photo Resistor

CRGB leds[NUM_LEDS];
CRGB ledsTarget[NUM_LEDS];
uint8_t brightness;


DateTime now; 
String hours, minutes, seconds;
int currentSecond, currentMinute, currentHour;
unsigned long timeToWait;
bool configMode;

// RTC code from: http://arduinolearning.com/code/arduino-ds3231-rtc-example.php
// Clock code from: https://github.com/mirohero/german-word-clock

void setup() {
  Serial.begin(9600);
  delay(3000); // wait for console opening
  Serial.println("Hello");
 
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //uncomment this line to set Time at ComplieTime.
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // When using PullUP Resistor, IF Pin gets connected to ground it will go to LOW
  pinMode(P1,INPUT_PULLUP);
  pinMode(P2,INPUT_PULLUP);

  brightness = 255; //Default brightness of the LEDs
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  fadeAll();
  configMode = false;
}

void fadeAll() {
  for (int i = 0; i < NUM_LEDS; i++) {
    ledsTarget[i] = CRGB(0,0,0);
  }
  FastLED.setBrightness(brightness);
  FastLED.show();
}
// Helper function that blends one uint8_t toward another by a given amount
void nblendU8TowardU8( uint8_t& cur, const uint8_t target, uint8_t amount)
{
  if( cur == target) return;
  
  if( cur < target ) {
    uint8_t delta = target - cur;
    delta = scale8_video( delta, amount);
    cur += delta;
  } else {
    uint8_t delta = cur - target;
    delta = scale8_video( delta, amount);
    cur -= delta;
  }
}
// Blend one CRGB color toward another CRGB color by a given amount.
// Blending is linear, and done in the RGB color space.
// This function modifies 'cur' in place.

void fadeTowardColor( CRGB& cur, const CRGB target, uint8_t amount)
{
  nblendU8TowardU8( cur.red,   target.red,   amount);
  nblendU8TowardU8( cur.green, target.green, amount);
  nblendU8TowardU8( cur.blue,  target.blue,  amount);
}

void fadeTowardsColorArray(CRGB* cur, CRGB target[],  uint8_t amount){
  for(int i = 0; i < NUM_LEDS; i++){
    fadeTowardColor(cur[i], target[i], amount);
  }
}

void showLed(int i, int h, int s, int v) {

  //leds[i] = fadeTowardColor (leds[i], CRGB(h,s,v), 5);
  ledsTarget[i] = CRGB(h, s, v);
}
void wordES( int h, int s, int v) {
  showLed(108, h, s, v); // S
  showLed(109, h, s, v); // Ä
}
void wordIST(int h, int s, int v) {
  showLed(103, h, s, v); // H
  showLed(104, h, s, v); // C
  showLed(105, h, s, v); // S
  showLed(106, h, s, v); // I
}
void wordFUENF(int h, int s, int v) {
  showLed(99, h, s, v);  // F
  showLed(100, h, s, v); // Ü
  showLed(101, h, s, v); // F
}
void wordZEHN(int h, int s, int v) {
  showLed(96, h, s, v); // z
  showLed(97, h, s, v); // Ä
  showLed(98, h, s, v); // H
}
void wordZWANZIG(int h, int s, int v) {
  showLed(82, h, s, v); // G
  showLed(83, h, s, v); // Z
  showLed(84, h, s, v); // N
  showLed(85, h, s, v); // Ä
  showLed(86, h, s, v); // W
  showLed(87, h, s, v); // Z
}
void wordVIERTEL(int h, int s, int v) {
  showLed(88, h, s, v); // V
  showLed(89, h, s, v); // I
  showLed(90, h, s, v); // Ä
  showLed(91, h, s, v); // R
  showLed(92, h, s, v); // T
  showLed(93, h, s, v); // E
  //showLed(94, h, s, v); // L

}
void wordNACH(int h, int s, int v) {
  showLed(66, h, s, v); // A
  showLed(67, h, s, v); // B
}
void wordVOR(int h, int s, int v) {
  showLed(77, h, s, v); // R
  showLed(78, h, s, v); // O
  showLed(79, h, s, v); // v
}
void wordZWOELF(int h, int s, int v) {
  showLed(0, h, s, v); // Z
  showLed(1, h, s, v); // W
  showLed(2, h, s, v); // Ö
  showLed(3, h, s, v); // L
  showLed(4, h, s, v); // F
  showLed(5, h, s, v); // I
}
void wordHALB(int h, int s, int v) {
  showLed(69, h, s, v); // H
  showLed(70, h, s, v); // A
  showLed(71, h, s, v); // L
  showLed(72, h, s, v); // B
  showLed(73, h, s, v); // I
}
void wordEINS(int h, int s, int v) {
  showLed(63, h, s, v); // S
  showLed(64, h, s, v); // I
  showLed(65, h, s, v); // E
}
void wordZWEI(int h, int s, int v) {
  showLed(59, h, s, v); // I
  showLed(60, h, s, v); // E
  showLed(61, h, s, v); // W
  showLed(62, h, s, v); // Z
}
void wordDREI(int h, int s, int v) {
  showLed(55, h, s, v); // Ü
  showLed(56, h, s, v); // R
  showLed(57, h, s, v); // D
}
void wordSIEBEN(int h, int s, int v) {
  showLed(33, h, s, v); // I
  showLed(34, h, s, v); // N
  showLed(35, h, s, v); // B
  showLed(36, h, s, v); // I
  showLed(37, h, s, v); // S
}
void wordSTUNDEDREI(int h, int s, int v) {
  showLed(55, h, s, v); // Ü
  showLed(56, h, s, v); // R
  showLed(57, h, s, v); // D
}
void wordSTUNDEFUENF(int h, int s, int v) {
  showLed(49, h, s, v); // F
  showLed(50, h, s, v); // Ü
  showLed(51, h, s, v); // F
  showLed(52, h, s, v); // I
}
void wordVIER(int h, int s, int v) {
  showLed(44, h, s, v); // V
  showLed(45, h, s, v); // I
  showLed(46, h, s, v); // E
  showLed(47, h, s, v); // R
  showLed(48, h, s, v); // I
}

void wordNEUN(int h, int s, int v) {
  showLed(27, h, s, v); // N
  showLed(28, h, s, v); // Ü 
  showLed(29, h, s, v); // N
  showLed(30, h, s, v); // I
}
void wordELF(int h, int s, int v) {
  showLed(11, h, s, v); // I
  showLed(12, h, s, v); // F
  showLed(13, h, s, v); // L
  showLed(14, h, s, v); // E
}
void wordACHT(int h, int s, int v) {
  showLed(22, h, s, v); // A
  showLed(23, h, s, v); // C
  showLed(24, h, s, v); // H
  showLed(25, h, s, v); // T
  showLed(26, h, s, v); // I
}
void wordSTUNDEZEHN(int h, int s, int v) {
  showLed(18, h, s, v); // I
  showLed(19, h, s, v); // N
  showLed(20, h, s, v); // Ä
  showLed(21, h, s, v); // Z
}
void wordSECHS(int h, int s, int v) {
  showLed(38, h, s, v); // I
  showLed(39, h, s, v); // S
  showLed(40, h, s, v); // H
  showLed(41, h, s, v); // C
  showLed(42, h, s, v); // Ä
  showLed(43, h, s, v); // S
}
void wordUHR(int h, int s, int v) {
  showLed(10, h, s, v); // R
  showLed(9, h, s, v); // H
  showLed(8, h, s, v); // U
}
void wordMinute1(int h, int s, int v){
  showLed(110, h, s, v);
}
void wordMinute2(int h, int s, int v){
  showLed(110, h, s, v);
  showLed(111, h, s, v);
}
void wordMinute3(int h, int s, int v){
  showLed(110, h, s, v);
  showLed(111, h, s, v);
  showLed(112, h, s, v);
}
void wordMinute4(int h, int s, int v){
  showLed(110, h, s, v);
  showLed(111, h, s, v);
  showLed(112, h, s, v);
  showLed(113, h, s, v);
}

int color() {
  return random(0, 255);
}

void updateLEDs(int amount = 255){
  fadeTowardsColorArray(leds, ledsTarget, amount); 
  FastLED.setBrightness(brightness);
  FastLED.show();
}

void loop() {

  now = rtc.now();  
  
  hours = now.hour();
  minutes = now.minute();
  seconds = now.second();

  brightness = analogRead(A0)/4; // Get brightness from photoresistor (Max 255)
  if (brightness < 10){ brightness = 10; }
  
  Serial.println("H:" + hours + " M:" + minutes + " S:" + seconds + " Brightness:" + brightness);
  currentHour = hours.toInt();
  currentMinute = minutes.toInt();
  currentSecond = seconds.toInt();
  wordES(color(), color(), color());
  wordIST(color(), color(), color());

  switch(currentMinute % 5){
    case 1:
      wordMinute1(color(), color(), color());
      break; 
    case 2:
      wordMinute2(color(), color(), color());
      break; 
    case 3:
      wordMinute3(color(), color(), color());
      break; 
    case 4:
      wordMinute4(color(), color(), color());
      break; 
  }
  switch (currentMinute/5){
    case 1: // 5-9 Min
      wordFUENF(color(), color(), color());
      wordNACH(color(), color(), color());
      break;
    case 2: // 10-14 Min
      wordZEHN(color(), color(), color());
      wordNACH(color(), color(), color());
      break;
    case 3: // 15-19 Min
      wordVIERTEL(color(), color(), color());
      wordNACH(color(), color(), color());
      break;
    case 4: // 20-24 Min
      wordZWANZIG(color(), color(), color());
      wordNACH(color(), color(), color());
      break;
    case 5: // 25-29 Min
      wordFUENF(color(), color(), color());
      wordVOR(color(), color(), color());
      wordHALB(color(), color(), color());
      currentHour += 1;
      break;
    case 6: // 30-34 Min
      wordHALB(color(), color(), color());
      currentHour += 1;
      break;
    case 7: // 35-39 Min
      wordFUENF(color(), color(), color());
      wordNACH(color(), color(), color());
      wordHALB(color(), color(), color());
      currentHour += 1;
      break;
    case 8: // 40-44 Min
      wordZWANZIG(color(), color(), color());
      wordVOR(color(), color(), color());
      currentHour += 1;
      break;
    case 9: // 45-49 Min
      wordVIERTEL(color(), color(), color());
      wordVOR(color(), color(), color());
      currentHour += 1;
      break;
    case 10: // 50-54 Min
      wordZEHN(color(), color(), color());
      wordVOR(color(), color(), color());
      currentHour += 1;
      break;
    case 11: // 55-59 Min
      wordFUENF(color(), color(), color());
      wordVOR(color(), color(), color());
      currentHour += 1;
      break;    
  }
  
  if (currentHour > 12){ currentHour -= 12; }
  switch (currentHour) {
    case 1:
      wordEINS(color(), color(), color());
      break;
    case 2:
      wordZWEI(color(), color(), color());
      break;
    case 3:
      wordSTUNDEDREI(color(), color(), color());
      break;
    case 4:
      wordVIER(color(), color(), color());
      break;
    case 5:
      wordSTUNDEFUENF(color(), color(), color());
      break;
    case 6:
      wordSECHS(color(), color(), color());
      break;
    case 7:
      wordSIEBEN(color(), color(), color());
      break;
    case 8:
      wordACHT(color(), color(), color());
      break;
    case 9:
      wordNEUN(color(), color(), color());
      break;
    case 10:
      wordSTUNDEZEHN(color(), color(), color());
      break;
    case 11:
      wordELF(color(), color(), color());
      break;
    case 12:
      wordZWOELF(color(), color(), color());
      break;
  }  
  // Exit Config Mode
  if((configMode == true) && (currentSecond >= 30)){
        Serial.println("EXIT config mode");
        configMode = false;
        wordUHR(0,0,0);
        fadeTowardsColorArray(leds, ledsTarget, 255); 
        FastLED.show();
  }
  if((digitalRead(P1) == HIGH) && (digitalRead(P2) == LOW)){
        configMode = true;
        wordUHR(255,0,0);  
        rtc.adjust(DateTime(now.year(), now.month(), now.day(), (now.hour()+1)%24, now.minute(), 0));
        Serial.println("Hours+1");
        
        updateLEDs();
        delay(500);   
  }
    // Adjust Minutes
  if((digitalRead(P1) == LOW) && (digitalRead(P2) == HIGH)){
        configMode = true;
        wordUHR(255,0,0);
        rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), (now.minute()+1)%60, 0));
        Serial.println("Minutes+1");
        
        updateLEDs();
        delay(500);
  }
  
  if(configMode == false){ //Normal operaion
        for (uint8_t x = 0; x < 100 ; x++){
          updateLEDs(10);
          FastLED.delay(10);
        }
        timeToWait = 60000 - currentSecond *1000;
        Serial.println(timeToWait);
        delay(timeToWait);
  } else  {
        updateLEDs();
        delay(500);
    }
    fadeAll();

}
