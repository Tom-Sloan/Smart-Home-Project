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
   Heart Rate Monitor Service:  0x180Da
   Heart Rate Measurement Char: 0x2A37
   Body Sensor Location Char:   0x2A38
   0x2aceab38cb354edcbc48f2510e458ea8
*/
BLEService        hrms = BLEService(0x8888);
BLECharacteristic hrmc = BLECharacteristic(UUID16_CHR_HEART_RATE_MEASUREMENT);
BLECharacteristic bslc = BLECharacteristic(UUID16_CHR_BODY_SENSOR_LOCATION);
BLECharacteristic toggleCUUID(0x0001);
BLEService        irUUID = BLEService(0x1111);

BLEDis bledis;    // DIS (Device Information Service) helper class instance
BLEBas blebas;    // BAS (Battery Service) helper class instance
uint8_t  bps = 0;
boolean canGrabValue = false;
int count = 0;

// Advanced function prototypes
void startAdv(void);
void setupHRM(void);
void connect_callback(uint16_t conn_handle);
void disconnect_callback(uint16_t conn_handle, uint8_t reason);

//light variables
bool greenLightStatus = true;
bool blueLightStatus = false;
int bluePin = 15;
int greenPin = 11;

int irPin = 27;
int timeDelay = millis();
boolean notSet = true;

void setup()
{
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(irPin, INPUT);
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb

  Serial.println("Smart Light");
  Serial.println("-----------------------\n");

  // Initialise the Bluefruit module
  Serial.println("Initialise the Bluefruit nRF52 module");
  Bluefruit.begin();

  // Set the advertised device name (keep it short!)
  Serial.println("Setting Device Name to 'Feather52 HRM'");
  Bluefruit.setName("Automatic Lights");

  // Set the connect/disconnect callback handlers
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);
  

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
  setupHRM(&hrms);

  // Setup the advertising packet(s)
  Serial.println("Setting up the advertising payload(s)");
  startAdv(&hrms);

  Serial.println("Ready Player One!!!");
  Serial.println("\nAdvertising");
}

void startAdv(BLEService* temp)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include HRM Service UUID
  Bluefruit.Advertising.addService(*temp);

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

void setupHRM(BLEService* temp)
{
  temp->begin();

  hrmc.setProperties(CHR_PROPS_NOTIFY);
  hrmc.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  hrmc.setFixedLen(2);
//  hrmc.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  hrmc.begin();
  uint8_t hrmdata[2] = { 0b00000110, 0x40 }; // Set the characteristic to use 8-bit values, with the sensor connected and detected
  hrmc.notify(hrmdata, 2);                   // Use .notify instead of .write!


  bslc.setProperties(CHR_PROPS_READ);
  bslc.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  bslc.setFixedLen(1);
  bslc.begin();
  bslc.write8(3);    // Set the characteristic to 'Wrist' (2)

  toggleCUUID.setProperties(CHR_PROPS_WRITE);
  toggleCUUID.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  toggleCUUID.setMaxLen(20);
  toggleCUUID.begin();


}

void connect_callback(uint16_t conn_handle)
{
  
//  char central_name[32] = { 0 };
//  Bluefruit.Gap.getPeerName(conn_handle, central_name, sizeof(central_name));

  Serial.println("Connected to ");
//  Serial.println(central_name);
  canGrabValue = true;
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
  canGrabValue = false;
  
}

/*
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

  canGrabValue = true;
}
*/
void loop()
{
  if(digitalRead(irPin) == LOW && notSet){
    Serial.println("DETECTED");
    setupHRM(&irUUID);
    startAdv(&irUUID);
    timeDelay = millis();
    notSet = false;
  }else if(millis()-timeDelay > 2000 && !notSet){
    notSet = true;
    setupHRM(&hrms);
    startAdv(&hrms);
  }

  if ( Bluefruit.connected() ) {
    if (canGrabValue) {
      delay(50);
      uint8_t read_value = toggleCUUID.read8();
      Serial.println(read_value);
      char loc_value = char(read_value);
      Serial.println(loc_value);
      int x = loc_value - '0';
      Serial.print("True Reading ");
      Serial.println(x);
      event(x);
      canGrabValue = false;
    }
  }
}

void event(int action) {
  if ( count == 0 ){
    action = 1;
    count++;
    Serial.println("using count at 0");
  }
  if ( greenLightStatus && !blueLightStatus){
    Serial.println("true and false");
  } else if (!greenLightStatus && blueLightStatus){
    Serial.println("false and true");
  }else{
    Serial.println("True and True or False and False");
  }
  if (action == 1) {
    if (greenLightStatus) {
      digitalWrite(greenPin, LOW);
      Serial.println("Green off, Blue On");
      digitalWrite(bluePin, HIGH);
      greenLightStatus = !greenLightStatus;
      blueLightStatus = !blueLightStatus;
    } else if (blueLightStatus) {
      digitalWrite(bluePin, LOW);
      Serial.println("Blue Off, Green On");
      digitalWrite(greenPin, HIGH);
      greenLightStatus = !greenLightStatus;
      blueLightStatus = !blueLightStatus;
    } else{
      Serial.print("Neither, here are the stati ");
    }
    
    Serial.println("Flipping Status");
  }
  else {
    Serial.println("Not action one, got action " +action);
  }
}
