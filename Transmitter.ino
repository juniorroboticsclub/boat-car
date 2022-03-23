 #include <SPI.h>
 #include <nRF24L01.h>
 #include <RF24.h>
 #define CE_PIN 9
 #define CSN_PIN 10
 #define JOYSTICK_X A0
 #define JOYSTICK_Y A1
#define PushButton 2 // this push button will be used to activate the lef right control or the up down control 
const uint64_t pipe = 0xE8E8F0F0E1LL;
 
RF24 radio(CE_PIN, CSN_PIN);
byte data[6]; // depending on the number of sensors used
 
int j1flag = 0; // joystick1 button flag
void setup()
 {
 Serial.begin(9600);
 radio.begin();
 radio.openWritingPipe(pipe);
 pinMode(JOYSTICK_X,INPUT);
 pinMode(JOYSTICK_Y, INPUT);
 pinMode(PushButton,INPUT_PULLUP);
 pinMode(A2, INPUT);
 
 
 // default values
data[0] = 127; // JOYSTICK1 VRX
data[1] = 127; // JOYSTICK1 VRY
data[2] = 1;  // jOYSTICK1 BUTTON
//data[3] = 1;  // SpeedPot
 
 
 }
void loop()
 {
 
data[0] = map(analogRead(JOYSTICK_X), 0, 1023, 0, 255); // joystick1 VRx
data[1] = map(analogRead(JOYSTICK_Y), 0, 1023, 0, 255); // Joystick1 VRy


if(digitalRead(PushButton) == LOW)
{
  data[2] = 1; 
}
if(digitalRead(PushButton) == HIGH)
{
  data[2] = 0; 
}
 
//data[3] = map(analogRead(A2),0 , 1023, 0, 255);  
Serial.println(data[1]);
Serial.println("Data");
Serial.println(data[0]); // up down
Serial.println(data[1]); // left right
Serial.println(data[2]); // button
//Serial.println(data[3]); // speed
delay(300);
radio.write( data, sizeof(data) );
 }
 
void resetData() 
{
 
}
