#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid = "HUAWEI P30 lite";
const char *password = "Nacho2020";
const char* mqtt_server = "test.mosquitto.org"; // Este es el address en mqtt dash
const char* keyDevice = "";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
int pinLed1 = 14; //Conectarlo al D0
int pinLed2 = 5; //Conectarlo al D1
int pinLed3 = 4; //Conectarlo al D2
int pinLed4 = 0; //Conectarlo al D3
void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("OK");
  }
void callback(char* topic, byte* payload, unsigned int length) {
 String topico = topic;
if(topico=="st/01")
{
    if (((char)payload[0] == '1')) { // 1 para prender
      digitalWrite(pinLed1,HIGH);
      Serial.println("Luz prendida");
  } else if (((char)payload[0] == '0')) { // 0 para apagar
      digitalWrite(pinLed1,LOW);
      Serial.println("Luz apagada");}
}
else if(topico=="st/02")
{
    if (((char)payload[0] == '1')) { // 1 para prender
      digitalWrite(pinLed2,HIGH);
      Serial.println("Luz prendida");
  } else if (((char)payload[0] == '0')) { // 0 para apagar
      digitalWrite(pinLed2,LOW);
      Serial.println("Luz apagada");}
}
else if(topico=="st/03")
{
    if (((char)payload[0] == '1')) { // 1 para prender
      digitalWrite(pinLed3,HIGH);
      Serial.println("Luz prendida");
  } else if (((char)payload[0] == '0')) { // 0 para apagar
      digitalWrite(pinLed3,LOW);
      Serial.println("Luz apagada");}
}
else if(topico=="st/04")
{
    if (((char)payload[0] == '1')) { // 1 para prender
      digitalWrite(pinLed4,HIGH);
      Serial.println("Luz prendida");
  } else if (((char)payload[0] == '0')) { // 0 para apagar
      digitalWrite(pinLed4,LOW);
      Serial.println("Luz apagada");}
}
}

void reconnect() {

  if (!client.connected()) {

    if (client.connect(keyDevice,"","")){//Servidor broker es el usuario y prueba12 la contraseña en mqtt dash
      Serial.println("connected");
      client.subscribe("st/#"); // Topico al momento de crear el boton
    } else {
      delay(5000);
    }
  }
}
void setup() {
  pinMode(pinLed1, OUTPUT);
  pinMode(pinLed2, OUTPUT);
  pinMode(pinLed3, OUTPUT);
  pinMode(pinLed4, OUTPUT);
  Serial.begin(9600);
  Serial.println("iniciando");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);   
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}


  
