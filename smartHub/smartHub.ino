/*
  new client
  POST / HTTP/1.1
  Host: 192.168.0.185
  Content-Type: application/json
  Connection: keep-alive
  Accept: /*
  User-Agent: Basic%20Chat/1 CFNetwork/974.2.1 Darwin/18.0.0
  Content-Length: 46
  Accept-Language: en-ca
  Accept-Encoding: gzip, deflate

  {"username":"@kilo_loco","tweet":"HelloWorld"}client disconnected

*/


#include <ArduinoJson.h>

#include <SPI.h>
#include <WiFi101.h>

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
boolean firstTime = true;

int ledPin1 = 10;
int ledPin2 = 11;
int ledPin3 = 12;
int ledPin4 = 13;

int timeDelay = 0;


WiFiServer server(80);

void setup() {
  Serial.println("SmartHub 1");
  pinSetup();
  WiFi.setPins(8, 7, 4, 2);
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

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
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        
        if (c == '{' && firstTime  && chars[0] == NULL) {
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

    if (firstTime && chars[0] != NULL ) {
      
      Serial.println("Running through parse statement");
      StaticJsonBuffer<400> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(chars);
      
      if (!root.success()) {
        Serial.println("parseObject() failed");
        return;
      }

      Serial.println("Parsed!");
      const char* id = root["id"];
      const char* action = root["action"];
      Serial.println(id);
      Serial.println(action);

      if (strcmp(id, "blinds") == 0) {

        if (strcmp(action, "up") == 0) {
          Serial.println("Opening BLinds!");
          digitalWrite(ledPin1, HIGH);
          delay(2000);
          digitalWrite(ledPin1, LOW);
        }
        if (strcmp(action, "down") == 0) {
          Serial.println("Closing BLinds!");
          digitalWrite(ledPin2, HIGH);
          delay(2000);
          digitalWrite(ledPin2, LOW);
        }

      } else if (strcmp(id, "light") == 0) {

        if (strcmp(action, "on") == 0) {
          Serial.println("Turing Light On");
          digitalWrite(ledPin3, HIGH);
          delay(2000);
          digitalWrite(ledPin3, LOW);
        }
        if (strcmp(action, "off") == 0) {
          Serial.println("Turing Light OFF");
          digitalWrite(ledPin3, HIGH);
          delay(2000);
          digitalWrite(ledPin3, LOW);
        }
        
      }
     
      firstTime = false;
    }

    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }

  if (millis() - timeDelay > 5000) {
    Serial.println("Can print again");
    firstTime = true;
    memset(chars, NULL, 40);
    Serial.print("Chars:");
    Serial.println(chars);
    Serial.print("FirstTime: ");
    Serial.println(firstTime);
    timeDelay = millis();
  }
}

















void pinSetup(){
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
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
