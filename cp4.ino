/* MEMBROS:
Gabriel C. S. da Silva RM: 563169
João Gabriel C. M. Santos RM: 563953
Leonardo V. de Souza RM: 562299
Miguel dos Santos de Andrade RM: 563516
Pedro H. da Silva RM: 566235
link Wokwi: https://wokwi.com/projects/441165407787101185
*/


#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
 
#define DHTPIN 15 // Pino GPIO15 do ESP32 para o DHT22
#define DHTTYPE DHT11 // Tipo de sensor DHT (DHT22)
DHT dht(DHTPIN, DHTTYPE);
 
#define LUM_PIN 34 // Pino GPIO34 do ESP32 para o Potenciômetro
 
// Credenciais
const char* ssid = "FIAP-IOT"; // Rede Wi-Fi
const char* password = "F!@p25.IOT"; // Senha da rede Wi-Fi
const char* apiKey = "RHMF75D9QRV3OV5J"; // Write API Key
const char* server = "http://api.thingspeak.com"; // Servidor ThingSpeak
 
void setup() {
  Serial.begin(115200);
  dht.begin();
 
  // Configuração dos pinos
  pinMode(LUM_PIN, INPUT);
 
  // Inicialização e loop de verificação da rede Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" conectado!");
}
 
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Leitura dos sensores
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int lumValue = analogRead(LUM_PIN); // Leitura do valor do Potenciômetro
    float lum = map(lumValue, 0, 4095, 0, 100); // Mapeamento do valor do potenciômetro para simular a velocidade de um carro da Fórmula E (0 a 322 km/h)
 
    if (isnan(h) || isnan(t)) {
      Serial.println("Falha ao ler o sensor DHT11!");
      return;
    }
 
    // Envio de dados para o ThingSpeak
    HTTPClient http;
    String url = String(server) + "/update?api_key=" + apiKey + "&field1=" + String(h) +
                                 "&field2=" + String(t) + "&field3=" + String(lum);
    http.begin(url);
 
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString(); // Resposta da requisição HTTP
      Serial.println("Dados enviados ao ThingSpeak.");
      Serial.print("Código HTTP: ");
      Serial.println(httpCode);
      Serial.println("Resposta: ");
      Serial.println(payload);
    } else {
      Serial.print("Erro ao enviar dados. Código HTTP: ");
      Serial.println(httpCode);
    }
   
    http.end();
  } else {
    Serial.println("WiFi não conectado. Tentando reconectar...");
  }
 
  // Espera 15 segundos para enviar a requisição novamente
  delay(2500);
}