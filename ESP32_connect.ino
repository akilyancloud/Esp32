#include <WiFi.h>        // Permet de gérer la connexion WiFi de l’ESP32
#include <HTTPClient.h> // Permet d’envoyer des requêtes HTTP (POST, GET…)
#include <WebServer.h>  // Permet de créer un serveur web sur l’ESP32

// ================= WIFI =================
const char* ssid = "NOMDERESEAU";      // Nom du réseau WiFi (à remplacer)
const char* password = "MOTDEPASSE";   // Mot de passe du WiFi (à remplacer)

// ================= SERVEUR PHP =================
const char* serverName = "http://IPDELAVM/DOSSIERVOULU/data.php"; 
// Adresse du serveur Ubuntu qui va recevoir les données (à modifier)

// ================= WEB SERVER =================
WebServer server(80); // Création d’un serveur web sur le port 80 (port HTTP)

// ================= PINS =================
const int led = 27;     // Broche utilisée pour la LED
const int buzzer = 26;  // Broche utilisée pour le buzzer

// ================= TIMER =================
unsigned long previousMillis = 0; // Stocke le dernier envoi de données
const long interval = 5000;       // Intervalle d’envoi (5 secondes)

// ================= WIFI =================
void connectWiFi() {
  WiFi.begin(ssid, password); // Démarre la connexion WiFi

  Serial.print("Connexion WiFi");

  // Boucle tant que l’ESP32 n’est pas connecté
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Une fois connecté
  Serial.println("\nConnecté !");
  Serial.println(WiFi.localIP()); // Affiche l’adresse IP de l’ESP32
}

// ================= ENVOI DONNÉES =================
void sendData() {

  int valeur = random(0, 100); // Génère une valeur aléatoire entre 0 et 100

  HTTPClient http;                 // Création d’un objet HTTP
  http.begin(serverName);          // Connexion à l’URL du serveur
  http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
  // Indique le format des données envoyées

  String postData = "valeur=" + String(valeur); 
  // Prépare les données à envoyer (format clé=valeur)

  int code = http.POST(postData); 
  // Envoie la requête POST au serveur

  Serial.println("--------------");
  Serial.print("Valeur envoyée: ");
  Serial.println(valeur); // Affiche la valeur envoyée

  Serial.print("HTTP code: ");
  Serial.println(code); // Affiche le code de réponse HTTP

  // Si la requête a réussi
  if (code > 0) {
    Serial.println(http.getString()); // Affiche la réponse du serveur
  }

  http.end(); // Ferme la connexion HTTP
}

// ================= LED =================
void handleLed() {

  digitalWrite(led, HIGH); // Allume la LED
  delay(300);              // Attend 300 ms
  digitalWrite(led, LOW);  // Éteint la LED

  server.send(200, "text/plain", "LED OK"); 
  // Envoie une réponse HTTP au navigateur
}

// ================= BUZZER =================
//CORRECTION IMPORTANTE ESP32 (CORE 3.x)
void handleBuzzer() {

  // initialisation correcte du PWM buzzer
  ledcAttach(buzzer, 2000, 8); 
  // Configure le buzzer avec une fréquence et une résolution

  ledcWriteTone(buzzer, 1000); // Joue un son (1000 Hz)
  delay(300);

  ledcWriteTone(buzzer, 500);  // Change la fréquence
  delay(300);

  ledcWriteTone(buzzer, 1500); // Encore une autre fréquence
  delay(300);

  ledcWriteTone(buzzer, 0);    // Arrête le son

  server.send(200, "text/plain", "BUZZER OK"); 
  // Réponse HTTP envoyée au client
}

// ================= SETUP =================
void setup() {

  Serial.begin(115200); // Initialise la communication série

  pinMode(led, OUTPUT);     // Configure la LED en sortie
  digitalWrite(led, LOW);   // S’assure que la LED est éteinte au départ

  connectWiFi(); // Connexion au WiFi

  // Définition des routes du serveur web ESP32
  server.on("/led", handleLed);      // Si /led est appelé → exécute handleLed
  server.on("/son", handleBuzzer);  // Si /son est appelé → exécute handleBuzzer

  server.begin(); // Démarre le serveur web

  Serial.println("Serveur ESP32 prêt");
}

// ================= LOOP =================
void loop() {

  server.handleClient(); 
  // Permet de gérer les requêtes entrantes (boutons web)

  // Vérifie si le WiFi est toujours connecté
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi(); // Reconnexion si nécessaire
    return;
  }

  // Envoie des données toutes les 5 secondes
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    sendData();
  }
}
