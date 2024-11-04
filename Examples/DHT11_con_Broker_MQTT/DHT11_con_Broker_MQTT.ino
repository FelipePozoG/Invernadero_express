#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Configuración del DHT11
#define DHTPIN 2    // Pin físico 2 (D2 en NodeMCU)
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Configuración de la red WiFi
const char* ssid = "5G2197";
const char* password = "agustin1234";

// Configuración del servidor Mosquitto (si usas uno local, cambia a tu IP)
const char* mqtt_server = "test.mosquitto.org";  // Cambia esto por la IP de tu servidor Mosquitto

// Definir los Topics
const char* topic_temperature = "home/dht11/temperature";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);  // Configura el broker Mosquitto
}

void setup_wifi() {
  delay(10);
  // Conectar a la red WiFi
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Reintentar conexión al broker MQTT hasta que se logre
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("Conectado al broker MQTT");
    } else {
      Serial.print("Error de conexión, rc=");
      Serial.print(client.state());
      Serial.println(" Reintentando en 5 segundos");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Leer los valores del sensor cada 2 segundos
  long now = millis();
  static long lastMsg = 0;
  if (now - lastMsg > 2000) {
    lastMsg = now;

    // Obtener temperatura y humedad
    float h = dht.readHumidity();
    float t = dht.readTemperature(); // Temperatura en °C

    // Verificar que las lecturas sean válidas
    if (isnan(h) || isnan(t)) {
      Serial.println("Error leyendo el DHT11");
      return;
    }

    // Publicar los datos en temas MQTT
    char tempStr[8];
    dtostrf(t, 6, 2, tempStr);
    Serial.print("Publicando temperatura: ");
    Serial.println(tempStr);
    client.publish("home/dht11/temperature", tempStr);

    char humStr[8];
    dtostrf(h, 6, 2, humStr);
    Serial.print("Publicando humedad: ");
    Serial.println(humStr);
    client.publish("home/dht11/humidity", humStr);
  }
}
