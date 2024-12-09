#ifndef CONTROL_SERVO_H
#define CONTROL_SERVO_H

#include <ESP32Servo.h>

// Definición de estados
typedef enum {
    Espera,
    Intensidad1,
    Intensidad2,
    Intensidad3,
    EMG
} Protesis;

// Declaraciones de variables globales
extern Servo servo;
extern int pinServo;
extern int pinEMG;
extern int signalEMG;
extern bool Bandera;
extern int medida;
extern int N;

extern int tiempo_intervalo;
extern int tiempo_serie;
extern int tiempo_millis;
extern unsigned long tiempo_real;
extern unsigned long tiempo_en_millis;
extern Protesis Intensidad;

// Declaraciones de funciones
void inicializarServo(int pin);
void manejarComando(String comando);
void ejecutarIntensidad();
void Tiempo_Encendido(unsigned long tiem);
void manejarEMG();

#endif
