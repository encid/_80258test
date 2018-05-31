/*
 * 216-80258-01 Pressure Assist board test
 * by R.Cavallaro  
 */
 
#include <LiquidCrystal.h>

#define DEBUG                        true     // flag to turn on/off debugging
#define DEBUG_PRINT(x) { if (DEBUG) { Serial.print(x);} }
#define DEBUG_PRINTLN(x) { if (DEBUG) { Serial.println(x);} }

const int HIGH_PRESS_FREQ_LOW_END =   80;
const int HIGH_PRESS_FREQ_HIGH_END =  100;
const int LOW_PRESS_FREQ_LOW_END =    220;
const int LOW_PRESS_FREQ_HIGH_END =   250;
const byte SWITCH_PIN = 6;
const byte FREQ_PIN = 9;
const byte FAIL_LED_PIN = 10;
const byte PASS_LED_PIN = 13;

byte pressureSwitch;    // storing digital input from switch for low or high pressure
float highTime;         // storing high time
float lowTime;          // storing low time
float totalTime;        // storing total time of a cycle
float frequency;        // storing frequency
String passFail = "";   // storing pass or fail string
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
    #if DEBUG
      Serial.begin(115200);
    #endif

    pinMode(FAIL_LED_PIN, OUTPUT);        // output for FAIL LED
    pinMode(PASS_LED_PIN, OUTPUT);        // output for PASS LED
    pinMode(FREQ_PIN, INPUT);             // pulse input for reading frequency
    pinMode(SWITCH_PIN, INPUT_PULLUP);    // mode select, LOW = check fully CW (high pressure) freq; high = check fully CCW (low pressure) freq

    lcd.display();                        // set up lcd
    lcd.begin(16,2);
}

void loop()
{       
    checkFrequency();
    delay(200);
}

void checkFrequency() {
    highTime = pulseIn(FREQ_PIN,HIGH);    // read high time
    lowTime = pulseIn(FREQ_PIN,LOW);      // read low time
    
    totalTime = highTime + lowTime;       // time between pulses in milliseconds
    frequency = 1000000 / totalTime;      // calculate frequency in Hz

    pressureSwitch = digitalRead(SWITCH_PIN);   // read pin 6 
    
    if (pressureSwitch == LOW) {          // if SWITCH_PIN is pulled low, check CW (high pressure) freq for pass/fail
      if (frequency > HIGH_PRESS_FREQ_LOW_END && frequency < HIGH_PRESS_FREQ_HIGH_END) {
        passFail = "PASS";
        digitalWrite(PASS_LED_PIN, HIGH);
        digitalWrite(FAIL_LED_PIN, LOW);
      }
      else {
        passFail = "FAIL";
        digitalWrite(PASS_LED_PIN, LOW);
        digitalWrite(FAIL_LED_PIN, HIGH);
      }
      lcd.setCursor(0,0);
      lcd.print(" HIGH PRESSURE  ");
      lcd.setCursor(0,1);
      lcd.print("   TEST: " + passFail);
      DEBUG_PRINTLN("Testing HIGH PRESSURE (FULL CW) Freq. - " + String(frequency) + " Hz . - " + passFail);
    }
    else if (pressureSwitch == HIGH) {    // if SWITCH_PIN is pulled high, check CCW (low pressure) freq for pass/fail
      if (frequency > LOW_PRESS_FREQ_LOW_END && frequency < LOW_PRESS_FREQ_HIGH_END) {
        passFail = "PASS";
        digitalWrite(PASS_LED_PIN, HIGH);
        digitalWrite(FAIL_LED_PIN, LOW);
      }
      else {
        passFail = "FAIL";
        digitalWrite(PASS_LED_PIN, LOW);
        digitalWrite(FAIL_LED_PIN, HIGH);
      }
      lcd.setCursor(0,0);
      lcd.print("  LOW PRESSURE  ");
      lcd.setCursor(0,1);
      lcd.print("   TEST: " + passFail);
      DEBUG_PRINTLN("Testing LOW PRESSURE (FULL CCW) Freq. - " + String(frequency) + " Hz . - " + passFail);
    }     
}
