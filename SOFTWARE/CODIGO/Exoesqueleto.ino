#include <ESP32Servo.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

Servo servo;
int pinServo = 13;
int pinEMG = 32;
int signalEMG = 0;
bool Bandera = 0;

// Definición de estados para la prótesis
typedef enum {
  Espera,
  Intensidad1,
  Intensidad2,
  Intensidad3,
  EMG
} Protesis;

Protesis Intensidad = Espera;

// Tarea para manejar el comando recibido por Serial
void serialTask(void *pvParameters) {
  while (true) {
    if (Serial.available() > 0) {
      String comando = Serial.readStringUntil('\n');
      comando.trim();
      if (comando == "Espera") {
        Intensidad = Espera;
      } else if (comando == "Intensidad1") {
        Intensidad = Intensidad1;
      } else if (comando == "Intensidad2") {
        Intensidad = Intensidad2;
      } else if (comando == "Intensidad3") {
        Intensidad = Intensidad3;
      } else if (comando == "EMG") {
        Intensidad = EMG;
      }
    }
    vTaskDelay(100 / portTICK_PERIOD_MS); // Esperar 100ms antes de revisar nuevamente
  }
}

// Tarea para controlar el servo basado en el estado de "Intensidad"
void servoTask(void *pvParameters) {
  while (true) {
    switch (Intensidad) {
      case Espera:
        servo.write(0);
        Serial.println("Modo_De_espera");
        break;

      case Intensidad1:
        servo.write(0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        servo.write(35);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        break;

      case Intensidad2:
        servo.write(0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        servo.write(50);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        break;

      case Intensidad3:
        servo.write(0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        servo.write(75);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        break;

      case EMG:
        signalEMG = analogRead(pinEMG);
        Serial.println(signalEMG);
        vTaskDelay(25 / portTICK_PERIOD_MS);
        if (signalEMG >= 4000) {
          if (Bandera) {
            servo.write(0);
            Bandera = !Bandera;
            vTaskDelay(2000 / portTICK_PERIOD_MS);
          } else {
            servo.write(75);
            Bandera = !Bandera;
            vTaskDelay(2000 / portTICK_PERIOD_MS);
          }
        }
        break;
    }
    vTaskDelay(50 / portTICK_PERIOD_MS); // Pequeña espera para no sobrecargar la CPU
  }
}

void setup() {
  // Configurar Serial y Servo
  Serial.begin(9600);
  servo.attach(pinServo, 500, 2500); // Definir los límites del servo

  // Crear tareas de FreeRTOS
  xTaskCreate(serialTask, "Serial Task", 2048, NULL, 1, NULL); // Manejar entrada serial
  xTaskCreate(servoTask, "Servo Task", 2048, NULL, 1, NULL);  // Controlar el servo
}

void loop() {
  // El loop principal queda vacío porque FreeRTOS maneja las tareas
}
