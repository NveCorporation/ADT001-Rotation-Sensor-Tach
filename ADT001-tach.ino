/****************************************************************************************  
Tachometer using NVE ADT001 noncontact magnetic rotation sensor
Arduino pin 2 to Sensor Sin output; pin 3 to Cos; pin 13 indicates direction; 
pins 7 to Rx on COM-11441 7-segment display for RPM; pin 5 is RPM variability PWM output
See https://www.nve.com/webstore/catalog/product_info.php?products_id=646 for sensor info
****************************************************************************************/       
#include <SoftwareSerial.h>
SoftwareSerial s7s(6, 7); //Pin 7 to Rx on 7-segment display
volatile int RPM, old_RPM, digit; //RPM calculations
volatile unsigned long edge_time, old_time; //Timer values

void setup() { 
pinMode(5, OUTPUT); //RPM variability PWM output
pinMode(13, OUTPUT); //Direction output
s7s.begin(9600); //9600 baud for 7-segment display
attachInterrupt(digitalPinToInterrupt(2), rpm, RISING); //Sensor generates interrupts
}
void loop() { //Display RPM
s7s.write(0x76); //Clear display
if (RPM<1000) s7s.print(" "); //Right justify
if (RPM<100) s7s.print(" "); 
if (RPM<10) s7s.print(" "); 
s7s.print(RPM); //Update display
delay(250);
}
void rpm(){ //Interrupt service routine calculates RPM 
  old_time=edge_time;
  edge_time=micros();
  digitalWrite(13, !digitalRead(3)); //Determine direction
  old_RPM=RPM;
  RPM=60000000L/(edge_time-old_time); //Calculate RPM
  analogWrite(5, constrain(abs(RPM-old_RPM), 0, 255)); //Output RPM change
  }
