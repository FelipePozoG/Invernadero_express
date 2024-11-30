#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Definir el tipo de sensor DHT
#define DHTPIN 2           // Pin donde se conecta el sensor DHT11 (GPIO2 en NodeMCU)
#define DHTTYPE DHT11      // Tipo de sensor (DHT11)
#define SOIL_SENSOR_PIN A0 // Pin analógico donde se conecta el sensor de humedad del suelo

// Inicializar el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);

// Inicializar el LCD (Dirección I2C 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Configuración de WiFi
const char* ssid = "Felipe";
const char* password = "12345678";
WiFiClient client;

// Datos de acceso para ThingSpeak
unsigned long numeroCanal = 2729542;
const char* WriteAPIKey = "IVV8DQN1Q5FO03TK";

// Temporizador para envío de datos y cambio de pantalla
unsigned long ultimoTiempoEnvio = 0;
unsigned long intervaloEnvio = 20000; // Cada 20 segundos
unsigned long ultimoTiempoPantalla = 0;
unsigned long intervaloPantalla = 4000; // Cambiar pantalla cada 4 segundos
bool mostrarTemperatura = true; // Alternar entre temperatura y humedad del suelo

void setup() {
  Serial.begin(115200);

  // Conectar a la red WiFi
  Serial.println("Conectando a WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado a WiFi");

  // Inicializar ThingSpeak
  ThingSpeak.begin(client);

  // Iniciar el LCD
  lcd.begin();
  lcd.clear();
  lcd.backlight();

  // Iniciar el sensor DHT
  dht.begin();

  // Mensaje inicial en el LCD
  lcd.setCursor(0, 0);
  lcd.print("DHT11 Sensor");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Leer la temperatura y la humedad del sensor DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Leer el valor de humedad del suelo
  int soilMoistureValue = analogRead(SOIL_SENSOR_PIN);

  // Mapear el valor de humedad del suelo (0 a 1023) a porcentaje (0 a 100%)
  int soilMoisturePercent = map(soilMoistureValue, 1023, 0, 0, 100);

  // Verificar si la lectura ha fallado en el DHT11
  if (isnan(h) || isnan(t)) {
    lcd.setCursor(0, 0);
    lcd.print("Error en sensor");
    Serial.println("Error en la lectura del sensor");
    return;
  }

  // Cambiar la pantalla cada 4 segundos
  if (millis() - ultimoTiempoPantalla >= intervaloPantalla) {
    ultimoTiempoPantalla = millis();
    mostrarTemperatura = !mostrarTemperatura;

    lcd.clear();
    if (mostrarTemperatura) {
      lcd.setCursor(0, 0);
      lcd.print("T. Aire: ");
      lcd.print(t);
      lcd.print(" C");

      lcd.setCursor(0, 1);
      lcd.print("H. Aire: ");
      lcd.print(h);
      lcd.print("%");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Humedad Suelo:");
      lcd.setCursor(0, 1);
      lcd.print(soilMoisturePercent);
      lcd.print("%");
    }
  }

  // Enviar datos a ThingSpeak cada 20 segundos
  if (millis() - ultimoTiempoEnvio >= intervaloEnvio) {
    ultimoTiempoEnvio = millis();

    ThingSpeak.setField(1, t);                   // Asigna temperatura al campo 1
    ThingSpeak.setField(2, h);                   // Asigna humedad al campo 2
    ThingSpeak.setField(3, soilMoisturePercent); // Asigna humedad del suelo al campo 3

    int statusCode = ThingSpeak.writeFields(numeroCanal, WriteAPIKey);
    if (statusCode == 200) {
      Serial.println("Datos enviados a ThingSpeak");
    } else {
      Serial.print("Error al enviar datos: ");
      Serial.println(statusCode);
    }
  }

  // Mostrar datos en el monitor serie
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" C");
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.println(" %");
  Serial.print("Humedad Suelo: ");
  Serial.print(soilMoisturePercent);
  Serial.println(" %");
}