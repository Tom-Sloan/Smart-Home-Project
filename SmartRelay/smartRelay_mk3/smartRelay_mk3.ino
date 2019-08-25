
/*********************************************************************
  This is an example for our nRF52 based Bluefruit LE modules

  Pick one up today in the adafruit shop!

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  MIT license, check LICENSE for more information
  All text above, and the splash screen below must be included in
  any redistribution
*********************************************************************/

/* This sketch show how to use BLEClientService and BLEClientCharacteristic
   to implement a custom client that is used to talk with Gatt server on
   peripheral.

   Note: you will need another feather52 running peripheral/custom_HRM sketch
   to test with.
*/

#include <bluefruit.h>

/* HRM Service Definitions
   Heart Rate Monitor Service:  0x180D
   Heart Rate Measurement Char: 0x2A37 (Mandatory)
   Body Sensor Location Char:   0x2A38 (Optional)
*/

BLEClientService        hrms(0x7777);
BLEClientService        blindUUID(0x9999);
BLEClientService        lightUUID(0x8888);

BLEClientCharacteristic responseCUUID(UUID16_CHR_HEART_RATE_MEASUREMENT);
BLEClientCharacteristic bslc(UUID16_CHR_BODY_SENSOR_LOCATION);
BLEClientCharacteristic toggleCUUID(0x0001);



BLEClientService* temp;
//BLEClientCharacteristic toggleCUUID(0x0001);
//BLEClientCharacteristic responseCUUID(0x0002);
//BLEClientCharacteristic textCUUID(0x0003);

int lightPin = 27;
int blindPin = 30;

boolean a = true;
boolean paired = false;
uint16_t conn;
int timeDelay = 0;
const int buttonPin = 31;     // the number of the pushbutton pin
const int ledPin =  LED_BUILTIN;      // the number of the LED pin


//Debouncing Variables
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers



void connectToDevice(BLEClientService* ble) {
  ble->begin();

  // Initialize client characteristics of HRM.
  // Note: Client Char will be added to the last service that is begin()ed.
  bslc.begin();

  // set up callback for receiving measurement
  responseCUUID.setNotifyCallback(hrm_notify_callback);
  responseCUUID.begin();

  toggleCUUID.begin();

  // Increase Blink rate to different from PrPh advertising mode
  Bluefruit.setConnLedInterval(250);

  // Callbacks for Central
  Bluefruit.Central.setDisconnectCallback(disconnect_callback);
  Bluefruit.Central.setConnectCallback(connect_callback);


  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(false);
  Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
  Bluefruit.Scanner.filterUuid(ble->uuid);
  Bluefruit.Scanner.useActiveScan(false);
  Bluefruit.Scanner.start(0);                   // // 0 = Don't stop scanning after n seconds
  temp = ble;
}


void setup()
{
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb

  Serial.println("Bluefruit52 Central Custom HRM Example");
  Serial.println("--------------------------------------\n");

  // Initialize Bluefruit with maximum connections as Peripheral = 0, Central = 1
  // SRAM usage required by SoftDevice will increase dramatically with number of connections
  Bluefruit.begin(0, 1);

  Bluefruit.setName("Bluefruit52 Central");

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  // set initial LED state
  digitalWrite(ledPin, ledState);
}


void loop()
{

  if (!Bluefruit.connected()){
    connectToDevice(&lightUUID);
  }else{
    sendEvent(1);
    Bluefruit.Central.disconnect(conn);
  }
//  int reading = digitalRead(buttonPin);
//  sendEvent(reading);
}

void sendEvent(int trigger) {

  if (trigger != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (trigger != buttonState) {
      buttonState = trigger;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        Serial.println(paired);
        ledState = !ledState;
        if (a) {

          if (paired) {
            Serial.println("Blind");
            connectToDevice(&blindUUID);
          } else if (!paired) {
            Serial.println("Light");
            connectToDevice(&lightUUID);
          }
          paired = !paired;

          a = false;
        } else if (!a) {
          Serial.print("TRIGGERED");
          a = true;
          Bluefruit.Central.disconnect(conn);
        }
      }
    }
  }
  // set the LED:
  digitalWrite(ledPin, ledState);

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = trigger;
}

void connect_callback(uint16_t conn_handle)
{
  conn = conn_handle;
  Serial.println("Connected");
  Serial.print("Discovering HRM Service ... ");

  // If HRM is not found, disconnect and return
  if ( !(temp->discover(conn_handle)) )
  {
    Serial.println("Found NONE");

    // disconect since we couldn't find HRM service
    Bluefruit.Central.disconnect(conn_handle);

    return;
  }

  // Once HRM service is found, we continue to discover its characteristic
  Serial.println("Found it");


  Serial.print("Discovering Measurement characteristic ... ");
  if ( !responseCUUID.discover() )
  {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("not found !!!");
    Serial.println("Measurement characteristic is mandatory but not found");
    Bluefruit.Central.disconnect(conn_handle);
    return;
  }
  Serial.println("Found it");

  // Measurement is found, continue to look for option Body Sensor Location
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.body_sensor_location.xml
  // Body Sensor Location is optional, print out the location in text if present
  Serial.print("Discovering Body Sensor Location characteristic ... ");
  if ( bslc.discover() )
  {
    Serial.println("Found it");

    // Body sensor location value is 8 bit
    const char* body_str[] = { "Other", "Chest", "Wrist", "Finger", "Hand", "Ear Lobe", "Foot" };

    // Read 8-bit BSLC value from peripheral
    uint8_t loc_value = bslc.read8();
    Serial.println(loc_value);
    Serial.print("Body Location Sensor: ");
    Serial.println(body_str[loc_value]);
  } else
  {
    Serial.println("Found NONE");
  }

  if ( toggleCUUID.discover() )
  {

    toggleCUUID.write8(4);

  } else
  {
    Serial.println("Found no toggle NONE");
  }

  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if ( responseCUUID.enableNotify() )
  {
    Serial.println("Ready to receive HRM Measurement value");
  } else
  {
    Serial.println("Couldn't enable notify for HRM Measurement. Increase DEBUG LEVEL for troubleshooting");
  }
}


void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  Serial.println("Disconnected");
}


/**
   Hooked callback that triggered when a measurement value is sent from peripheral
   @param chr   Pointer client characteristic that even occurred,
                in this example it should be responseCUUID
   @param data  Pointer to received data
   @param len   Length of received data
*/
void hrm_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.heart_rate_measurement.xml
  // Measurement contains of control byte0 and measurement (8 or 16 bit) + optional field
  // if byte0's bit0 is 0 --> measurement is 8 bit, otherwise 16 bit.

  Serial.print("HRM Measurement: ");

  if ( data[0] & bit(0) )
  {
    uint16_t value;
    memcpy(&value, data + 1, 2);

    Serial.println(value);
  }
  else
  {
    Serial.println(data[1]);
  }
}

void scan_callback(ble_gap_evt_adv_report_t* report)
{
  // Since we configure the scanner with filterUuid()
  // Scan callback only invoked for device with hrm service advertised
  // Connect to device with HRM service in advertising
  Bluefruit.Central.connect(report);
}
