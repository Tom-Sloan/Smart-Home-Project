#include <ArduinoJson.h>
#include <SPI.h>
#include <WiFi101.h>
#include <Wire.h> //sending to address 9


char ssid[] = "SAM3VirtualHouse";     //  your network SSID (name)
char pass[] = "16137635795";          // your network password
//char ssid[] = "Scottish Castle";
//char pass[] = "!007Bond";

int keyIndex = 0; // your network key Index number (needed only for WEP)
String response;
int status = WL_IDLE_STATUS;
boolean toStore = false;
char chars[40];
int count = 0;

int blindUp = 10;
int blindDown = 11;
int lightOn = 13;
int lightOff = 12;

int timeDelay = 0;


WiFiServer server(80);

void setup() {
  Wire.begin();

  WiFi.setPins(8, 7, 4, 2);
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  //  while (!Serial) {
  //    ; // wait for serial port to connect. Needed for native USB port only
  //  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();
  // you're connected now, so print out the status:
  printWiFiStatus();
}



void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        if (c == '{'  && chars[0] == NULL) {
          toStore = true;
        }

        if (toStore) {
          chars[count] = c;
          count++;
        }

        if (c == '}') {
          toStore = false;
          break;
        }
      }
    }

    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
  Serial.print("SUCCESS1");
  StaticJsonBuffer<400> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(chars);
  Serial.print("SUCCESS2");
  if (!root.success()) {

    return;
  }

  Serial.println("Parsed!");
  const char* id = root["id"];
  const char* action = root["action"];
  const char* bleId = root["bleId"];

  Serial.print("id: ");
  Serial.println(id);
  Serial.print("action: ");
  Serial.println(action);
  Serial.print("bleId: ");
  Serial.println(bleId);
  if (strcmp(id, "Movement") == 0) {
    Serial.println("Movement");
    char* con;
    con = (char*)malloc(strlen(action) + strlen(bleId) + strlen(id) + 2);
    strcpy(con, action);
    strcpy(con, bleId);
    strcpy(con, id);
    Wire.beginTransmission(9); // transmit to device #9
    Wire.write(action);
    Wire.write(bleId);
    Wire.endTransmission();
    Serial.println("doing stuff!");
    free(con);
    timeDelay = millis();

  } else {
    Serial.println("Sending");
    const char* bleId = root["bleId"];
    char* con;
    con = (char*)malloc(strlen(action) + strlen(bleId) + 2);
    strcpy(con, action);
    strcpy(con, bleId);
    strcpy(con, "a");
    Wire.beginTransmission(9); // transmit to device #9
    Wire.write(action);
    Wire.write(bleId);
    Wire.endTransmission();
    Serial.println("doing stuff!");
    free(con);
  }
  for (int i = 0; i < count; i++) {
    chars[i] = NULL;
  }
  count = 0;
  id = NULL;
  action = NULL;

}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}