# Projeto MQTT e Mosquitto

# Explicação do Código - Controle de LEDs via MQTT com NodeMCU

## 1. Inclusão das bibliotecas

```cpp
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
```

- **ESP8266WiFi.h**: Permite conectar o NodeMCU (ESP8266) à rede Wi-Fi.  
- **PubSubClient.h**: Biblioteca para comunicação MQTT (publicar, assinar tópicos, etc).  
- **ArduinoJson.h**: Biblioteca para criar e manipular mensagens JSON.

----------------------------------------------------

## 2. Configurações Wi-Fi e MQTT

```cpp
const char* ssid = "Funcionapfvr";       
const char* password = "62838700";       
const char* mqtt_server = "192.168.215.189"; 
const int mqtt_port = 1883;               
```

Define as credenciais da rede Wi-Fi e o endereço do servidor MQTT (broker) para se conectar.

----------------------------------------------------

## 3. Definição dos tópicos MQTT e pinos dos LEDs

```cpp
const char* sub_topics[] = { ... };
const char* pub_topics[] = { ... };
const uint8_t led_pins[] = {D5, D4, D3, D2, D1};
const int num_leds = 5;
```

- **sub_topics**: tópicos que o NodeMCU irá "ouvir" para receber comandos.  
- **pub_topics**: tópicos que o NodeMCU usará para enviar o status atual (ON/OFF).  
- **led_pins**: os pinos do NodeMCU ligados fisicamente aos LEDs.

----------------------------------------------------

## 4. Objetos para Wi-Fi e MQTT

```cpp
WiFiClient espClient;
PubSubClient client(espClient);
```

- Cria o cliente Wi-Fi.  
- Usa esse cliente para criar a conexão MQTT.

----------------------------------------------------

## 5. Função `setup_wifi()`

```cpp
void setup_wifi() {
  // Conecta o NodeMCU à rede Wi-Fi
}
```

- Inicia a conexão Wi-Fi.  
- Espera até conectar.  
- Exibe no Serial o IP obtido.

----------------------------------------------------

## 6. Função `callback()`

```cpp
void callback(char* topic, byte* payload, unsigned int length) {
  // Recebe mensagens MQTT
  // Interpreta comando ON/OFF para cada tópico (LED)
  // Liga/desliga o LED correspondente
  // Publica o estado atual no tópico de status em formato JSON
}
```

- Recebe o tópico e a mensagem.  
- Transforma a mensagem recebida em texto.  
- Verifica para qual LED o tópico se refere.  
- Liga ou desliga o LED conforme o comando "ON" ou "OFF".  
- Publica o status em JSON (ex: `{ "estado": "ON" }`).

----------------------------------------------------

## 7. Função `reconnect()`

```cpp
void reconnect() {
  // Tenta conectar ao broker MQTT
  // Se conectar, assina os tópicos de comando
  // Se falhar, tenta novamente depois de 5 segundos
}
```

- Garante reconexão ao MQTT caso perca a conexão.  
- Registra-se nos tópicos que receberá comandos.

----------------------------------------------------

## 8. Função `setup()`

```cpp
void setup() {
  Serial.begin(9600);
  // Inicializa os pinos dos LEDs como saída, desligados
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}
```

- Configura hardware e comunicação.  
- Configura callback para receber mensagens MQTT.

----------------------------------------------------

## 9. Função `loop()`

```cpp
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
```

- Verifica a conexão MQTT constantemente.  
- Processa mensagens recebidas (chama o `callback` quando chegar mensagem).

----------------------------------------------------

## FOTOS
![Imagem do WhatsApp de 2025-06-09 à(s) 21 31 20_fb63a734](https://github.com/user-attachments/assets/0495fc68-be73-457a-a6b5-29e2b9c86a8a)
![Imagem do WhatsApp de 2025-06-09 à(s) 21 31 18_5dc05349](https://github.com/user-attachments/assets/c6490add-8114-4451-b3a6-422f4c02f272)
![Imagem do WhatsApp de 2025-06-09 à(s) 21 31 26_760f26ae](https://github.com/user-attachments/assets/0a1882ae-51b0-4115-aaab-5736803fbb6e)



##
