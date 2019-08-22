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
#include <bluefruit.h>

/* HRM Service Definitions
   Heart Rate Monitor Service:  0x180D
   Heart Rate Measurement Char: 0x2A37
   Body Sensor Location Char:   0x2A38
*/
BLEService        hrms = BLEService(0x6666);
BLECharacteristic hrmc = BLECharacteristic(UUID16_CHR_HEART_RATE_MEASUREMENT);
BLECharacteristic bslc = BLECharacteristic(UUID16_CHR_BODY_SENSOR_LOCATION);
BLECharacteristic blind = BLECharacteristic(0x0001); //0000 -> can't be used
BLECharacteristic light = BLECharacteristic(0X0002);

BLEDis bledis;    // DIS (Device Information Service) helper class instance
BLEBas blebas;    // BAS (Battery Service) helper class instance

// Button Variables will change:
int buttonState = 0;
int timeDelay = 0;
int timeCounter = 0;
float buttonPressedTimer = 0;
bool buttonPressed = false;
bool canSendAgain = true;

//Accerleration Variabels
int lastX, lastY, lastZ = 0;
float scaledX, scaledY, scaledZ;
int rawX, rawY, rawZ;
double average;

//Step counter variables
int stepCounter = 0;
float threshold = 1.00;

// Constants won't change. They're used here to set pin numbers:
const int buttonPin = 27;     // the number of the pushbutton pin
const int ledPin =  LED_BUILTIN;         // the number of the LED pin
const int scale = 3;

uint8_t  bps = 0;
uint8_t  lightStatus = 0;
uint8_t  blindStatus = 0;

// Advanced function prototypes
void startAdv(void);
void setupHRM(void);
void connect_callback(uint16_t conn_handle);
void disconnect_callback(uint16_t conn_handle, uint8_t reason);

void setup()
{
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb

  Serial.println("Bluefruit52 HRM Example");
  Serial.println("-----------------------\n");

  // Initialise the Bluefruit module
  Serial.println("Initialise the Bluefruit nRF52 module");
  Bluefruit.begin();

  // Set the advertised device name (keep it short!)
  Serial.println("Setting Device Name to 'Feather52 HRM'");
  Bluefruit.setName("SmartBand");

  // Set the connect/disconnect callback handlers
  Bluefruit.setConnectCallback(connect_callback);
  Bluefruit.setDisconnectCallback(disconnect_callback);

  // Configure and Start the Device Information Service
  Serial.println("Configuring the Device Information Service");
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather52");
  bledis.begin();

  // Start the BLE Battery Service and set it to 100%
  Serial.println("Configuring the Battery Service");
  blebas.begin();
  blebas.write(100);

  // Setup the Heart Rate Monitor service using
  // BLEService and BLECharacteristic classes
  Serial.println("Configuring the Heart Rate Monitor Service");
  setupHRM();

  // Setup the advertising packet(s)
  Serial.println("Setting up the advertising payload(s)");
  startAdv();

  Serial.println("Ready Player One!!!");
  Serial.println("\nAdvertising");
  digitalPinStarter();

  getAccelerationData(rawX, rawY, rawZ);
  average = sqrt(scaledZ * scaledZ + scaledX * scaledX + scaledY * scaledY);

}

void loop()
{

  if ( Bluefruit.connected() ) {


    //Takes the accleration data and sets the scaled version
    getAccelerationData(rawX, rawY, rawZ);
    printAccelerationData();
    checkForButtonPress();


    if (buttonPressed) {
      checkForAction();
    }
    //  float totAccel = sqrt(scaledX * scaledX + scaledY * scaledY + scaledZ * scaledZ);
    //
    //  if(totAccel > threshold && millis() - timeCounter > 100){
    //    char str[16];
    //    stepCounter += 1;
    //    sprintf(str, "%d", stepCounter);
    //    bleuart.write(str);
    //  }

    if (millis() - timeCounter > 100) {
      timeCounter = millis();
      lastX = scaledX;
      lastY = scaledY;
      lastZ = scaledZ;
    }
  }

  Serial.println();
}

void checkForAction() {
  if (canSendAgain) {
    delay(100);
    char str[16];
    double current = sqrt(scaledZ * scaledZ + scaledX * scaledX + scaledY * scaledY);
    Serial.print("Current : "); Serial.print(current); Serial.print(" m/s\t");

    int y = fabs(scaledY) - fabs(lastY);
    int z = fabs(scaledZ) - fabs(lastZ);

    if (y < z) {
      if (scaledY > (lastY + .4) && fabs(current - average) > .05) {
        Serial.print("Lights ON\n");
        lightStatus = 3;
        sendData(light, lightStatus);
        canSendAgain = false;

      } else if (scaledY < (lastY - .4)  && fabs(current - average) > .05) {
        Serial.print("Lights Off\n");
        lightStatus = 4;
        sendData(light, lightStatus);
        canSendAgain = false;
      }
    }
    if (z > y) {
      if (scaledZ > (lastZ + .4) && fabs(current - average) > .05) {
        Serial.print("Blinds Up\n");
        blindStatus = 1;
        sendData(blind, blindStatus);
        canSendAgain = false;
      } else if (scaledZ < (lastZ - .4) && fabs(current - average) > .05) {
        Serial.print("Blinds down\n");
        blindStatus = 2;
        sendData(blind, blindStatus);
        canSendAgain = false;

      }
    }
  }
}

void sendData(BLECharacteristic BLEChar, int data) {
  //first value is the flag value. I can be set to legnth of array - 2 (therefore it could also be 0 currently) or that value(only tested for length 2)
  uint8_t dataArr[2] = { 0b00000110, data };           // Sensor connected, increment BPS value

  // Note: We use .notify instead of .write!
  // If it is connected but CCCD is not enabled
  // The characteristic's value is still updated although notification is not sent

  if (BLEChar.notify(dataArr, sizeof(dataArr))) {
    //Serial.print("Heart Rate Measurement updated to: "); Serial.println(data);

  } else {
    //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
  }
}



//************************************Stuff I don't use often**********************************************************************************
//Setup pin mode for the digital pins
void digitalPinStarter() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

//Gets the acceleration data in the raw format and float maps it to the proper scaled version.
//NOTE: this only works if it is 3.3V. Use 675 if 5V.
void getAccelerationData(int rawX, int rawY, int rawZ) {

  //Creats the raw accleration data
  rawX = analogRead(A0);
  rawY = analogRead(A1);
  rawZ = analogRead(A2);

  scaledX = mapf(rawX, 0, 1023, -scale, scale);
  scaledY = mapf(rawY, 0, 1023, -scale, scale);
  scaledZ = mapf(rawZ, 0, 1023, -scale, scale) ;

}

//Prints scalled versions of x, y, z acceleration
void printAccelerationData() {
  Serial.print("X: "); Serial.print(scaledX); Serial.print(" m/s\t");
  //Serial.print("X: "); Serial.print(rawX); Serial.print(" m/s\t");
  Serial.print("Y: "); Serial.print(scaledY); Serial.print(" m/s\t");
  //Serial.print("Y: "); Serial.print(rawY); Serial.print(" m/s\t");
  Serial.print("Z: "); Serial.print(scaledZ); Serial.print(" m/s\t");
  //Serial.print("Z: "); Serial.print(rawZ); Serial.println(" m/s\t");

  Serial.print("Average: "); Serial.print(average); Serial.print(" m/s\t");

}

//maps floats
float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//Checks to see if the button has been pressed.
//If it has already been pressed it will set it to false after five seconds
void checkForButtonPress() {

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    sendData(hrmc, 1);
    buttonPressed = true;
    Serial.print("ON BUTTON! ");
    Serial.println(buttonState);
    digitalWrite(ledPin, HIGH);
    buttonPressedTimer = millis();
  }

  if ((millis() - buttonPressedTimer) / 1000 > 3) {
    buttonPressed = false;
    canSendAgain = true;
    sendData(hrmc, 0);
    digitalWrite(ledPin, LOW);
  }
}


void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include HRM Service UUID
  Bluefruit.Advertising.addService(hrms);

  // Include Name
  Bluefruit.Advertising.addName();

  /* Start Advertising
     - Enable auto advertising if disconnected
     - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
     - Timeout for fast mode is 30 seconds
     - Start(timeout) with timeout = 0 will advertise forever (until connected)

     For recommended advertising interval
     https://developer.apple.com/library/content/qa/qa1931/_index.html
  */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}

void setupHRM(void)
{
  // Configure the Heart Rate Monitor service
  // See: https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.heart_rate.xml
  // Supported Characteristics:
  // Name                         UUID    Requirement Properties
  // ---------------------------- ------  ----------- ----------
  // Heart Rate Measurement       0x2A37  Mandatory   Notify
  // Body Sensor Location         0x2A38  Optional    Read
  // Heart Rate Control Point     0x2A39  Conditional Write       <-- Not used here
  hrms.begin();

  // Note: You must call .begin() on the BLEService before calling .begin() on
  // any characteristic(s) within that service definition.. Calling .begin() on
  // a BLECharacteristic will cause it to be added to the last BLEService that
  // was 'begin()'ed!

  // Configure the Heart Rate Measurement characteristic
  // See: https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.heart_rate_measurement.xml
  // Properties = Notify
  // Min Len    = 1
  // Max Len    = 8
  //    B0      = UINT8  - Flag (MANDATORY)
  //      b5:7  = Reserved
  //      b4    = RR-Internal (0 = Not present, 1 = Present)
  //      b3    = Energy expended status (0 = Not present, 1 = Present)
  //      b1:2  = Sensor contact status (0+1 = Not supported, 2 = Supported but contact not detected, 3 = Supported and detected)
  //      b0    = Value format (0 = UINT8, 1 = UINT16)
  //    B1      = UINT8  - 8-bit heart rate measurement value in BPM
  //    B2:3    = UINT16 - 16-bit heart rate measurement value in BPM
  //    B4:5    = UINT16 - Energy expended in joules
  //    B6:7    = UINT16 - RR Internal (1/1024 second resolution)
  hrmc.setProperties(CHR_PROPS_NOTIFY);
  hrmc.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  hrmc.setFixedLen(2);
  hrmc.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  hrmc.begin();
  uint8_t hrmdata[2] = { 0b00000110, 0x40 }; // Set the characteristic to use 8-bit values, with the sensor connected and detected
  hrmc.notify(hrmdata, 2);                   // Use .notify instead of .write!

  blind.setProperties(CHR_PROPS_NOTIFY);
  blind.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  blind.setFixedLen(2);
  blind.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  blind.begin();
  uint8_t blindData[2] = { 0b00000110, 0x40 }; // Set the characteristic to use 8-bit values, with the sensor connected and detected
  blind.notify(blindData, 2);

  light.setProperties(CHR_PROPS_NOTIFY);
  light.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  light.setFixedLen(2);
  light.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  light.begin();
  uint8_t lightData[2] = { 0b00000110, 0x40 }; // Set the characteristic to use 8-bit values, with the sensor connected and detected
  light.notify(lightData, 2);

  // Configure the Body Sensor Location characteristic
  // See: https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.body_sensor_location.xml
  // Properties = Read
  // Min Len    = 1
  // Max Len    = 1
  //    B0      = UINT8 - Body Sensor Location
  //      0     = Other
  //      1     = Chest
  //      2     = Wrist
  //      3     = Finger
  //      4     = Hand
  //      5     = Ear Lobe
  //      6     = Foot
  //      7:255 = Reserved
  bslc.setProperties(CHR_PROPS_READ);
  bslc.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  bslc.setFixedLen(1);
  bslc.begin();
  bslc.write8(2);    // Set the characteristic to 'Wrist' (2)
}

void connect_callback(uint16_t conn_handle)
{
  char central_name[32] = { 0 };
  Bluefruit.Gap.getPeerName(conn_handle, central_name, sizeof(central_name));

  Serial.print("Connected to ");
  Serial.println(central_name);
}

/**
   Callback invoked when a connection is dropped
   @param conn_handle connection where this event happens
   @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
*/
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  Serial.println("Disconnected");
  Serial.println("Advertising!");
}

void cccd_callback(BLECharacteristic& chr, uint16_t cccd_value)
{
  // Display the raw request packet
  Serial.print("CCCD Updated: ");
  //Serial.printBuffer(request->data, request->len);
  Serial.print(cccd_value);
  Serial.println("");

  // Check the characteristic this CCCD update is associated with in case
  // this handler is used for multiple CCCD records.
  if (chr.uuid == hrmc.uuid) {
    if (chr.notifyEnabled()) {
      Serial.println("Heart Rate Measurement 'Notify' enabled");
    } else {
      Serial.println("Heart Rate Measurement 'Notify' disabled");
    }
  }
  if (chr.uuid == blind.uuid) {
    if (chr.notifyEnabled()) {
      Serial.println("Blind 'Notify' enabled");
    } else {
      Serial.println("Blind 'Notify' disabled");
    }
  }
  if (chr.uuid == light.uuid) {
    if (chr.notifyEnabled()) {
      Serial.println("Light 'Notify' enabled");
    } else {
      Serial.println("Light 'Notify' disabled");
    }
  }
}
