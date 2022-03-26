#include <SPI.h>
#include <Servo.h>
#include <nRF24L01.h>
#include <RF24.h>
#define CE_PIN 9
#define CSN_PIN 10


Servo motorleft;
Servo motorright;



int mode = 0;

int ud_data; // up down data
int lr_data; // left right data
int speed_data; // brushless motor speed data
int speed_data1;
int speed_data2;
int speed_data3;
int speed_data4;

int receiver_reset = 8;



#define in1 7 //Motor1  L298 Pin in1 
#define in2 3 //Motor1  L298 Pin in1 
#define in3 2 //Motor2  L298 Pin in1 
#define in4 4 //Motor2  L298 Pin in1 



const uint64_t pipe = 0xE8E8F0F0E1LL;

RF24 radio(CE_PIN, CSN_PIN);
byte data[6]; // depending on the number of sensors used


unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

int secs = 0;

void setup()
{
  digitalWrite(receiver_reset, HIGH);
  delay(200);
  pinMode(receiver_reset, OUTPUT);
  Serial.begin(9600);



  motorleft.attach(5,1000,2000);
  motorright.attach(6,1000,2000);


  Serial.println("Nrf24L01 Receiver Starting");

  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  resetData();

  //default values
  data[0] = 127;
  data[1] = 127;


  motorleft.write(0);
  motorright.write(0);

  pinMode(in1, OUTPUT); // declare as output for L298 Pin in1
  pinMode(in2, OUTPUT); // declare as output for L298 Pin in2
  pinMode(in3, OUTPUT); // declare as output for L298 Pin in3
  pinMode(in4, OUTPUT); // declare as output for L298 Pin in4

}

void loop()
{
 

  if ( radio.available() )
  {
    radio.read( data, sizeof(data));

    Serial.println("Data");
    Serial.println(data[0]); // up down
    Serial.println(data[1]); // left right
    Serial.println(data[2]); // button

    if (data[2] == 1)
    {
      mode = 0;
    }
    // Left Right

    if (data[2] == 0)
    {
      mode = 1;
    }
  }
  else
  {
    currentTime = millis();
    if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
      resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone has a throttle up and we lose connection, it can keep flying unless we reset the values

      //

      secs = secs + 1;
      if (secs >= 2)
      {
        digitalWrite( receiver_reset, LOW);
        secs = 0;
      }

    }
  }



  if (mode == 0)
  {
  

    if (data[0] < 20)
    {
      forword();
    }
    else if (data[0] > 200)
    {
      backword();
    }
    else if (data[1] < 20)
    {
      turnLeft();
    }
    else if (data[1] > 200)
    {
      turnRight();
    }
    else {
      Stop();
    }

  }

  if (mode == 1) {

    if (data[0] < 110)
    {
      boatforword();
    }
    else if (data[1] < 110)
    {
      boatleft();
    }
    else if (data[1] > 200)
    {
      boatright();
    }
    else
    {
      boatstop();
    }







  }





}

void resetData()
{
  // we are going to place our default code over here.
  Serial.println("Connection Lost");
  Stop();
  motorleft.writeMicroseconds(1000);
  motorright.writeMicroseconds(1000);

}



void forword() { //forword
  Serial.println("forword");
  digitalWrite(in1, HIGH); //Right Motor forword Pin
  digitalWrite(in2, LOW);  //Right Motor backword Pin
  digitalWrite(in3, LOW);  //Left Motor backword Pin
  digitalWrite(in4, HIGH); //Left Motor forword Pin


}

void backword() { //backword
  Serial.println("backword");
  digitalWrite(in1, LOW);  //Right Motor forword Pin
  digitalWrite(in2, HIGH); //Right Motor backword Pin
  digitalWrite(in3, HIGH); //Left Motor backword Pin
  digitalWrite(in4, LOW);  //Left Motor forword Pin


}

void turnRight() { //turnRight
  Serial.println("turnRight");
  digitalWrite(in1, LOW);  //Right Motor forword Pin
  digitalWrite(in2, HIGH); //Right Motor backword Pin
  digitalWrite(in3, LOW);  //Left Motor backword Pin
  digitalWrite(in4, HIGH); //Left Motor forword Pin


}
void turnLeft() { //turnLeft
  Serial.println("turnLeft");
  digitalWrite(in1, HIGH);  //Right Motor forword Pin
  digitalWrite(in2, LOW); //Right Motor backword Pin
  digitalWrite(in3, HIGH);  //Left Motor backword Pin
  digitalWrite(in4, LOW); //Left Motor forword Pin

}
void Stop() { //stop
  Serial.println("stop");
  digitalWrite(in1, LOW); //Right Motor forword Pin
  digitalWrite(in2, LOW); //Right Motor backword Pin
  digitalWrite(in3, LOW); //Left Motor backword Pin
  digitalWrite(in4, LOW); //Left Motor forword Pin

}


void boatforword()
{
  Serial.println("boatforword");
  speed_data = map(data[0], 0, 125, 0, 180);
  motorleft.writeMicroseconds(speed_data);
  motorright.writeMicroseconds(speed_data);
  delay(50);

}

void boatleft()
{
  Serial.println("boatleft");
  speed_data1 = map(data[1], 0, 125, 0, 180);
  motorleft.writeMicroseconds(speed_data1);
  speed_data2 = map(data[1], 0, 125, 180, 0);
  motorright.writeMicroseconds(speed_data2);
  delay(50);

}
void boatright()
{

  Serial.println("boatright");
  speed_data3 = map(data[1], 130, 255, 180, 0);
  motorleft.writeMicroseconds(speed_data3);
  speed_data4 = map(data[1], 130, 255, 0, 180);
  motorright.writeMicroseconds(speed_data4);
  delay(50);

}
void boatstop()
{
  Serial.println("boatstop");
  motorleft.writeMicroseconds(1000);
  motorright.writeMicroseconds(1000);
  delay(50);

}
