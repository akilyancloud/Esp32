#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>

// ================= WIFI =================
const char* ssid = "TPSN035";
const char* password = "BTSSN2022";

// ================= SERVEUR PHP =================
const char* serverName = "http://192.168.100.132/btsciel/data.php";

// ================= WEB SERVER =================
WebServer server(80);

// ================= PINS =================
const int led = 27;
const int buzzer = 26;

// ================= TIMER =================
unsigned long previousMillis = 0;
const long interval = 5000;

// ================= WIFI =================
void connectWiFi() {
  WiFi.begin(ssid, password);

  Serial.print("Connexion WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnecté !");
  Serial.println(WiFi.localIP());
}

// ================= ENVOI DONNÉES =================
void sendData() {

  int valeur = random(0, 100);

  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String postData = "valeur=" + String(valeur);

  int code = http.POST(postData);

  Serial.println("--------------");
  Serial.print("Valeur envoyée: ");
  Serial.println(valeur);

  Serial.print("HTTP code: ");
  Serial.println(code);

  if (code > 0) {
    Serial.println(http.getString());
  }

  http.end();
}

// ================= LED =================
void handleLed() {

  digitalWrite(led, HIGH);
  delay(300);
  digitalWrite(led, LOW);

  server.send(200, "text/plain", "LED OK");
}

// ================= BUZZER =================
//CORRECTION IMPORTANTE ESP32 (CORE 3.x)
void handleBuzzer() {

  // initialisation correcte du PWM buzzer
  ledcAttach(buzzer, 2000, 8);

  ledcWriteTone(buzzer, 1000);
  delay(300);

  ledcWriteTone(buzzer, 500);
  delay(300);

  ledcWriteTone(buzzer, 1500);
  delay(300);

  ledcWriteTone(buzzer, 0);

  server.send(200, "text/plain", "BUZZER OK");
}

// ================= SETUP =================
void setup() {

  Serial.begin(115200);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  connectWiFi();

  // routes web ESP32
  server.on("/led", handleLed);
  server.on("/son", handleBuzzer);

  server.begin();

  Serial.println("Serveur ESP32 prêt");
}

// ================= LOOP =================
void loop() {

  server.handleClient();

  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
    return;
  }

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    sendData();
  }
}
