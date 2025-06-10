#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "Funcionapfvr";
const char* password = "62838700";
const char* mqtt_server = "192.168.215.189";
const int mqtt_port = 1883;

const char* sub_topics[] = {
  "casa/cozinha/led/set",
  "casa/quarto1/led/set",
  "casa/quarto2/led/set",
  "casa/varanda/led/set",
  "casa/sala/led/set"
};

const char* pub_topics[] = {
  "casa/cozinha/led/status",
  "casa/quarto1/led/status",
  "casa/quarto2/led/status",
  "casa/varanda/led/status",
  "casa/sala/led/status"
};

const uint8_t led_pins[] = {D5, D4, D3, D2, D1};
const int num_leds = 5;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  Serial.print("Conectando na rede Wi-Fi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado! IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  msg.trim();
  msg.toUpperCase();

  for (int i = 0; i < num_leds; i++) {
    if (String(topic) == sub_topics[i]) {
      if (msg == "ON") {
        digitalWrite(led_pins[i], HIGH);
        Serial.printf("LED %d ligado via tópico %s\n", i, topic);
      } else if (msg == "OFF") {
        digitalWrite(led_pins[i], LOW);
        Serial.printf("LED %d desligado via tópico %s\n", i, topic);
      } else {
        Serial.printf("Comando inválido para %s: %s\n", topic, msg.c_str());
      }

      // Criar e publicar JSON
      StaticJsonDocument<64> doc;
      doc["estado"] = msg;
      char buffer[64];
      size_t n = serializeJson(doc, buffer);
      client.publish(pub_topics[i], buffer, n);
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "NodeMCU-" + String(random(0xffff), HEX);
    Serial.print("Tentando conectar MQTT com ID: ");
    Serial.println(clientId);
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado ao broker MQTT!");
      for (int i = 0; i < num_leds; i++) {
        client.subscribe(sub_topics[i]);
        Serial.print("Inscrito em: ");
        Serial.println(sub_topics[i]);
      }
    } else {
      Serial.print("Falha. Código: ");
      Serial.print(client.state());
      Serial.println(" – tentando novamente em 5s");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("=== Iniciando NodeMCU + MQTT ===");

  for (int i = 0; i < num_leds; i++) {
    pinMode(led_pins[i], OUTPUT);
    digitalWrite(led_pins[i], LOW);
  }

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
