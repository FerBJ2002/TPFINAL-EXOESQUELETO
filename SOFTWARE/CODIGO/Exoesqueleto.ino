#include <ESP32Servo.h>

Servo servo;
int pinServo = 13;
int pinEMG = 32;
int signalEMG = 0;
bool Bandera = 0;
int medida = 0;
int N = 5;

int tiempo_intervalo = 0;  // Variable para el tiempo de intervalo
int tiempo_serie = 0;      // Variable para el tiempo de serie
int tiempo_millis;
unsigned long tiempo_real;
unsigned long tiempo_en_millis;

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
    comando.trim();  // Limpiar posibles espacios en blanco al principio o final

    // Buscar el primer separador ":" para dividir los datos
    int primer_colon = comando.indexOf(':');
    if (primer_colon > 0) {
      String modo = comando.substring(0, primer_colon);  // Obtener el modo (Intensidad1, Intensidad2, etc.)
      String datos = comando.substring(primer_colon + 1);  // Obtener los datos restantes (tiempo de intervalo y serie)

      // Verificar el modo de intensidad y asignarlo
      if (modo == "Espera") {
        Intensidad = Espera;
      }
      else if (modo == "Intensidad1") {
        Intensidad = Intensidad1;
      }
      else if (modo == "Intensidad2") {
        Intensidad = Intensidad2;
      }
      else if (modo == "Intensidad3") {
        Intensidad = Intensidad3;
      }
      else if (modo == "EMG") {
        Intensidad = EMG;
      }

      // Dividir los tiempos (intervalo y serie) usando otro ":"
      int segundo_colon = datos.indexOf(':');
      if (segundo_colon > 0) {
        String intervalo_str = datos.substring(0, segundo_colon);
        String serie_str = datos.substring(segundo_colon + 1);

        // Convertir los valores de tiempo a enteros
        tiempo_intervalo = intervalo_str.toInt();
        tiempo_serie = serie_str.toInt();
        tiempo_millis = tiempo_intervalo*1000;
      }
    }
    
  }

  // Ejecutar la acción correspondiente dependiendo del valor de Intensidad
  switch (Intensidad) {
    case Espera:
      servo.write(5);
      Serial.println("Modo_De_espera");
      tiempo_en_millis = millis();
      break;

    case Intensidad1:
      servo.write(5);
      delay(tiempo_millis);  // Usar tiempo de intervalo (en segundos)
      servo.write(35);
      delay(tiempo_millis);  // Usar tiempo de serie (en segundos)
      Tiempo_Encendido(tiempo_serie);
      break;

    case Intensidad2:
      servo.write(5);
      delay(tiempo_millis);
      servo.write(50);
      delay(tiempo_millis);
      Tiempo_Encendido(tiempo_serie);
      break;

    case Intensidad3:
      servo.write(5);
      delay(tiempo_millis);
      servo.write(75);
      delay(tiempo_millis);
      Tiempo_Encendido(tiempo_serie);
      break;

    case EMG:
      medida = medida * (N-1)/N + analogRead(pinEMG)/N;
      Serial.println(medida);
      delay(50);
      if (medida >= 10) {
        if (Bandera) {
          servo.write(5);
          Bandera = !Bandera;
          delay(2000);
        } else {
          servo.write(73);
          Bandera = !Bandera;
          delay(2000);
        }
      }
      break;
  }
}

void Tiempo_Encendido(unsigned long tiem){
  tiempo_real = tiem*1000;
  if((millis()-tiempo_en_millis)>= tiempo_real){
    Serial.println("Terminado");
    Intensidad = Espera;
    
  }

}