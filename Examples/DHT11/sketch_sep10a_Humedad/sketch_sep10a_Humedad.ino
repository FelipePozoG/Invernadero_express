#include "DHT.h"  //Incluir libreria.

#define DHTPIN 4     // Pin donde está conectado el sensor.

#define DHTTYPE DHT22   // Sensor DHT22.

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200); // Iniciar puerto serial en 115200.
  Serial.println("Iniciando...");
  dht.begin();
}
void loop() {
  delay(6000);
  float h = dht.readHumidity(); // Leer humedad.
  float t = dht.readTemperature(); // Leer temperatura en grados celcius.
  // Muestreo de datos en puerto serial 9600.
  Serial.print("Humedad:"); 
  Serial.print(h);
  Serial.println(" %t");  
  // Serial.print("\t"); // Espacio entre letras.
  Serial.print("Temperatura: ");  // "println" Linea de codigo con salto de linea.
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print('\n'); // Imprimir salto de linea.
}
