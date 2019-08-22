
#include <bluefruit.h>
// The SFE_LSM9DS1 library requires both Wire and SPI be
// included BEFORE including the 9DS1 library.
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>

#define LSM9DS1_M  0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW
#define DECLINATION -13.01 // Declination (degrees) in Boulder, CO.

BLEService        hrms = BLEService(0x6666);
BLECharacteristic hrmc = BLECharacteristic(UUID16_CHR_HEART_RATE_MEASUREMENT);
BLECharacteristic bslc = BLECharacteristic(UUID16_CHR_BODY_SENSOR_LOCATION);
BLECharacteristic control = BLECharacteristic(0x0001); //0000 -> can't be used


BLEDis bledis;    // DIS (Device Information Service) helper class instance
BLEBas blebas;    // BAS (Battery Service) helper class instance

LSM9DS1 imu;

// Button Variables will change:
int buttonState = 0;
float buttonPressedTimer = 0;
float recordingTime = .5;
bool buttonPressed = false;
bool canSendAgain = true;
bool hasCalibrated = false;
float timeBetween = 1.5;

int num_of_rec = 0;
double acc_x = 0;
double acc_y = 0;
double acc_z = 0;
double gyr_x = 0;
double gyr_y = 0;
double gyr_z = 0;
double mag_x = 0;
double mag_y = 0;
double mag_z = 0;
int val[36];

double timeBetweenAccelRead = 0;
double lastReadingTime = 0;


//Step counter variables
int stepCounter = 0;
float threshold = 1.00;

// Constants won't change. They're used here to set pin numbers:
const int buttonPin = 27;     // the number of the pushbutton pin
const int ledPin =  LED_BUILTIN;         // the number of the LED pin
const int scale = 3;

uint8_t  bps = 0;

// Advanced function prototypes
void startAdv(void);
void setupHRM(void);
void connect_callback(uint16_t conn_handle);
void disconnect_callback(uint16_t conn_handle, uint8_t reason);

void setup()
{
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb

  Serial.println("SmartBand Serial");
  Serial.println("-----------------------\n");

  // Initialise the Bluefruit module
  Serial.println("Initialise the Bluefruit nRF52 module");
  Bluefruit.begin();

  // Set the advertised device name (keep it short!)
  Serial.println("Setting Device Name to 'SmartBand'");
  Bluefruit.setName("SmartBand");

  // Set the connect/disconnect callback handlers
  Bluefruit.setConnectCallback(connect_callback);
  Bluefruit.setDisconnectCallback(disconnect_callback);

  // Configure and Start the Device Information Service
  Serial.println("Configuring the Device Information Service");
  bledis.setManufacturer("XTech");
  bledis.setModel("Universal 1");
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

  Serial.println("\nAdvertising");
  digitalPinStarter();

  startIMU();
  Serial.println("Ending setup");

}

void loop()
{
  
  if ( Bluefruit.connected() ) {
    //updates button val
    //checkForButtonPress();
    sendData(hrmc, 2);
    
//    if (buttonPressed) {
//      checkForAction();
//    } else {
//
//      timeBetweenAccelRead = 0;
//      lastReadingTime = millis();
//    }
    

  }

}
