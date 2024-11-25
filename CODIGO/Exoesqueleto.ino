#include <ESP32Servo.h>
Servo servo;
int pinServo=13;
int pinEMG = 32;
int signalEMG = 0;
bool Bandera = 0;
void setup(){
  Serial.begin(9600);
  servo.attach(pinServo,500,2500);
}
void loop(){
  signalEMG = analogRead(pinEMG);
  Serial.println(signalEMG);
  delay(25);
  if(signalEMG >= 4000){
    if(Bandera){
      servo.write(5);
      Bandera = !Bandera;
      delay(2000);
    }else{
      servo.write(75);
      Bandera = !Bandera;
      delay(2000);
    }
  }
}