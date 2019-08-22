/*Todo:
  Dynamic passcodes
  Adjustable size for chars[]
  Make a function for the i2c transmission
  Do I need to parse the json? could i jest send stright istead of separating then putting together
*/
#include <ArduinoJson.h>
#include <SPI.h>
#include <WiFi101.h>
#include <Wire.h> //sending to address 9

//Passwords
char ssid[] = "SAM3VirtualHouse";     //  your network SSID (name)
char pass[] = "16137635795";          // your network password
//char ssid[] = "Scottish Castle";
//char pass[] = "!007Bond";

String response;
int status = WL_IDLE_STATUS;
boolean toStore = false; // Used for json parsing
char chars[40]; // What the json parsing gets stored into (Watch size)
int count = 0; // Used to keep track of how many chars are in chars[]

int timeDelay = 0;//Dont think this is needed


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
    //Records the json data. The data we want is between {} so we ignore the rest
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        //Start recording
        if (c == '{'  && chars[0] == NULL) {
          toStore = true;
        }
        //Record
        if (toStore) {
          chars[count] = c;
          Serial.println(chars);
          count++;
        }
        //Stop Recording
        if (c == '}') {
          toStore = false;
          Serial.println(chars);
          break;
        }
      }
      
    }
    
    // give the web browser time to receive the data
    delay(1000);
    Serial.println("client disconnected");
    Serial.println(chars);
    // close the connection:
    client.stop();
    
    
  }
  
  //Parses the json
  StaticJsonBuffer<400> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(chars);
  

  if (!root.success()) {
    return;
  }
  Serial.println("Parsed!");

  //Set values to json values
  const char* id = root["id"];
  const char* action = root["action"];
  const char* bleId = root["bleId"];

  Serial.print("id: ");
  Serial.println(id);
  Serial.print("action: ");
  Serial.println(action);
  Serial.print("bleId: ");
  Serial.println(bleId);

  //can only send 1 string per transmission therefore the strings are merged
  //Then strings are sent to the ble device
  //the if is just for proving security
  if (strcmp(id, "Movement") == 0) {
    Serial.println("Movement");
    char* con;
    const char* a = " ";
    con = (char*)malloc(strlen(action) + 1 + strlen(bleId) + 1 + strlen(id) + 2);
    strcpy(con, action);
    strcpy(con, a);
    strcpy(con, bleId);
    strcpy(con, a);
    strcpy(con, id);
    Serial.print("con: ");
    Serial.println(con);
    Wire.beginTransmission(9); // transmit to device #9
    Wire.write(con);
    Wire.endTransmission();
    Serial.println("doing stuff!");
    free(con);
    timeDelay = millis();
  }


  //Empties the array that contains the chars
  for (int i = 0; i < count; i++) {
    chars[i] = NULL;
  }
  count = 0;
  id = NULL;
  action = NULL;
  bleId = NULL;
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
