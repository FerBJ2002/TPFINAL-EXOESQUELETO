#include <ESP32Servo.h>

Servo servo;
int pinServo = 13;
int pinEMG = 32;
int signalEMG = 0;
bool Bandera = 0;

typedef enum {
  Espera,
  Intensidad1,
  Intensidad2,
  Intensidad3,
  EMG
} Protesis;

Protesis Intensidad = Espera;

void setup() {
  // Inicializar comunicación serial y servo
  Serial.begin(9600);
  servo.attach(pinServo, 500, 2500);  // Definir los límites del servo
}

void loop() {

  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();
    if (comando == "Espera") {
      Intensidad = Espera;
    }
    else if (comando == "Intensidad1") {
      Intensidad = Intensidad1;
    }
    else if (comando == "Intensidad2") {
      Intensidad = Intensidad2;
    }
    else if (comando == "Intensidad3") {
      Intensidad = Intensidad3;
    }
    else if (comando == "EMG") {
      Intensidad = EMG;
    }
  }

  // Ejecutar la acción correspondiente dependiendo del valor de Intensidad
  switch (Intensidad) {
    case Espera:
      servo.write(0);
      Serial.println("Modo_De_espera");
      break;
      
    case Intensidad1:
      servo.write(0);
      delay(1000);
      servo.write(35);
      delay(1000);
      break;
      
    case Intensidad2:
      servo.write(0);
      delay(1000);
      servo.write(50);
      delay(1000);
      break;
      
    case Intensidad3:
      servo.write(0);
      delay(1000);
      servo.write(75);
      delay(1000);
      break;
      
    case EMG:
      signalEMG = analogRead(pinEMG);
      Serial.println(signalEMG);
      delay(25);
      if (signalEMG >= 4000) {
        if (Bandera) {
          servo.write(0);
          Bandera = !Bandera;
          delay(2000);
        } else {
          servo.write(75);
          Bandera = !Bandera;
          delay(2000);
        }
      }
      break;
  }
}