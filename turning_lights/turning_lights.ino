// IMPLEMENTACIÓN DE UN FILTRO DE COMPLEMENTO ACELERÓMETRO + GIROSCOPIO
// Librerias I2C para controlar el mpu6050
// la libreria MPU6050.h necesita I2Cdev.h, I2Cdev.h necesita Wire.h
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

//#define DEBUG

// La dirección del MPU6050 puede ser 0x68 o 0x69, dependiendo 
// del estado de AD0. Si no se especifica, 0x68 estará implicito
MPU6050 sensor;

// Valores RAW (sin procesar) del acelerometro y giroscopio en los ejes x,y,z
int ax, ay, az;
int gx, gy, gz;

const int hist = 4;
const int range = 8;
const int mean = 0;

const int leftLed = 2;
const int rightLed = 3;

long tiempo_prev;
float dt;
float ang_x, ang_y;
float ang_x_prev, ang_y_prev;

void setup() {
#ifdef DEBUG
  Serial.begin(9600);    //Iniciando puerto serial
#endif
  Wire.begin();           //Iniciando I2C  
  sensor.initialize();    //Iniciando el sensor

#ifdef DEBUG
  if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al iniciar el sensor");
#endif

  pinMode(leftLed, OUTPUT);
  pinMode(rightLed, OUTPUT);
}

void loop() {
  // Leer las aceleraciones y velocidades angulares
  sensor.getAcceleration(&ax, &ay, &az);
  sensor.getRotation(&gx, &gy, &gz);
  
  dt = (millis()-tiempo_prev)/1000.0;
  tiempo_prev=millis();
  
  //Calcular los ángulos con acelerometro
  float accel_ang_x=atan(ay/sqrt(pow(ax,2) + pow(az,2)))*(180.0/3.14);
  float accel_ang_y=atan(-ax/sqrt(pow(ay,2) + pow(az,2)))*(180.0/3.14);
  
  //Calcular angulo de rotación con giroscopio y filtro complemento  
  ang_x = 0.7*(ang_x_prev+(gx/131)*dt) + 0.3*accel_ang_x;
  ang_y = 0.7*(ang_y_prev+(gy/131)*dt) + 0.3*accel_ang_y;
  
  
  ang_x_prev=ang_x;
  ang_y_prev=ang_y;

  //Mostrar los angulos separadas por un [tab]
#ifdef DEBUG
  Serial.print("Rotacion en X:  ");
  Serial.print(ang_x); 
  Serial.print(" Rotacion en Y: ");
  Serial.println(ang_y);
#endif

  if(ang_y >= (mean + range))
  {
    digitalWrite(leftLed, LOW); 
    digitalWrite(rightLed, HIGH); 
  }
  else if(ang_y <= (mean - range))
  {
    digitalWrite(leftLed, HIGH); 
    digitalWrite(rightLed, LOW); 
  }
  else
  {
    if((ang_y <= (mean + range - hist)&&(ang_y >= (mean - range + hist))))
    {
      digitalWrite(leftLed, LOW); 
      digitalWrite(rightLed, LOW);  
    }
  }  
  delay(100);
}
