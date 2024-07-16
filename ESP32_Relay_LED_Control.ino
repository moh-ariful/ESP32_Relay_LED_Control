#include <WiFi.h>

#define RELAY_PIN 27
#define LED_PIN 33 // Tambahkan ini untuk LED

const char* ssid = "myhome";
const char* password = "12345678";

WiFiServer server(80);

bool relayState = false;
String header;

// Enum untuk level log
enum LogLevel {
  INFO,
  WARN,
  ERROR,
  NONE
};

LogLevel currentLogLevel = INFO;

void logMessage(LogLevel level, const String &message) {
  if (level >= currentLogLevel) {
    Serial.println(message);
  }
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    logMessage(INFO, ".");
    retries++;
    yield();
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    logMessage(INFO, "\nWiFi terhubung.");
    logMessage(INFO, "SSID: " + WiFi.SSID());
    logMessage(INFO, "IP address: " + WiFi.localIP().toString());
    server.begin();
    logMessage(INFO, "Memulai Server");
  } else {
    logMessage(ERROR, "\nGagal terhubung ke WiFi.");
  }
}

void setRelayState(bool state) {
  relayState = state;
  digitalWrite(RELAY_PIN, state ? LOW : HIGH);
  digitalWrite(LED_PIN, state ? HIGH : LOW); // Tambahkan ini untuk mengontrol LED
  logMessage(INFO, state ? "Relay dinyalakan" : "Relay dimatikan");
}

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT); // Konfigurasi pin LED sebagai output
  setRelayState(false); // Pastikan relay mati saat awal

  Serial.begin(115200);
  logMessage(INFO, "");
  logMessage(INFO, "Mulai Konfigurasi");

  connectToWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    logMessage(WARN, "Koneksi WiFi terputus! Mencoba menghubungkan ulang...");
    connectToWiFi();
  }

  WiFiClient client = server.available();

  if (client) {
    logMessage(INFO, "Client Baru");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html; charset=UTF-8");
            client.println("Connection: close");
            client.println();
            
            client.println("<!DOCTYPE html><html lang=\"id\">");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<meta charset=\"UTF-8\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>");
            client.println("html { font-family: Helvetica; height: 100%; margin: 0; }");
            client.println("body { text-align: center; display: flex; flex-direction: column; justify-content: flex-start; height: 100%; }");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; }");
            client.println(".button2 { background-color: #555555; }");
            client.println(".header-text { color: #00008B; }");
            client.println("</style></head>");
            
            client.println("<body><h1 class='header-text'>Next Koding</h1>");
            client.println("<h2>ESP32 Web Server</h2>");

            if (header.indexOf("GET /26/on") >= 0) {
              setRelayState(true);
            } else if (header.indexOf("GET /26/off") >= 0) {
              setRelayState(false);
            }

            client.println("<p>Status Relay: " + String(relayState ? "Nyala" : "Mati") + "</p>");
            if (relayState) {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">Matikan Relay</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">Hidupkan Relay</button></a></p>");
            }

            client.println("</body></html>");
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    logMessage(INFO, "Client Terputus");
  }

  // Batasi output serial dengan interval 30 detik
  static unsigned long lastPrintTime = 0;
  unsigned long currentTime = millis();
  if (currentTime - lastPrintTime > 59000) {
    logMessage(INFO, "SSID: " + WiFi.SSID());
    logMessage(INFO, "IP address: " + WiFi.localIP().toString());
    lastPrintTime = currentTime;
  }

  yield();
}
