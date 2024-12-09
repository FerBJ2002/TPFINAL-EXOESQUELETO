#include <Arduino.h>
#include "control_servo.h"

void setup() {
    // Inicializar Serial y el servo
    Serial.begin(9600);
    inicializarServo(pinServo);
}

void loop() {
    // Leer comando desde Serial
    if (Serial.available() > 0) {
        String comando = Serial.readStringUntil('\n');
        manejarComando(comando);
    }

    // Ejecutar la acción según el estado
    ejecutarIntensidad();
}
