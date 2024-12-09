#include "control_servo.h"

// Variables globales
Servo servo;
int pinServo = 13;
int pinEMG = 32;
int signalEMG = 0;
bool Bandera = 0;
int medida = 0;
int N = 5;

int tiempo_intervalo = 0;
int tiempo_serie = 0;
int tiempo_millis;
unsigned long tiempo_real;
unsigned long tiempo_en_millis;
Protesis Intensidad = Espera;

// Función para inicializar el servo
void inicializarServo(int pin) {
    servo.attach(pin, 500, 2500);
}

// Función para manejar el comando recibido vía Serial
void manejarComando(String comando) {
    comando.trim();

    int primer_colon = comando.indexOf(':');
    if (primer_colon > 0) {
        String modo = comando.substring(0, primer_colon);
        String datos = comando.substring(primer_colon + 1);

        if (modo == "Espera") {
            Intensidad = Espera;
        } else if (modo == "Intensidad1") {
            Intensidad = Intensidad1;
        } else if (modo == "Intensidad2") {
            Intensidad = Intensidad2;
        } else if (modo == "Intensidad3") {
            Intensidad = Intensidad3;
        } else if (modo == "EMG") {
            Intensidad = EMG;
        }

        int segundo_colon = datos.indexOf(':');
        if (segundo_colon > 0) {
            String intervalo_str = datos.substring(0, segundo_colon);
            String serie_str = datos.substring(segundo_colon + 1);

            tiempo_intervalo = intervalo_str.toInt();
            tiempo_serie = serie_str.toInt();
            tiempo_millis = tiempo_intervalo * 1000;
        }
    }
}

// Función para manejar la lógica según el estado
void ejecutarIntensidad() {
    switch (Intensidad) {
        case Espera:
            servo.write(5);
            Serial.println("Modo_De_espera");
            tiempo_en_millis = millis();
            break;

        case Intensidad1:
            servo.write(5);
            delay(tiempo_millis);
            servo.write(35);
            delay(tiempo_millis);
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
            manejarEMG();
            break;
    }
}

// Función para verificar el tiempo encendido
void Tiempo_Encendido(unsigned long tiem) {
    tiempo_real = tiem * 1000;
    if ((millis() - tiempo_en_millis) >= tiempo_real) {
        Serial.println("Terminado");
        Intensidad = Espera;
    }
}

// Función para manejar los datos del sensor EMG
void manejarEMG() {
    medida = medida * (N - 1) / N + analogRead(pinEMG) / N;
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
}
