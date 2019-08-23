
#include <bluefruit.h>
#include <Wire.h>

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
bool hasCalibrated = false;
int timeBetween = 2;

double timeBetweenAccelRead = 0;
double lastReadingTime = 0;
double velX, velY, velZ = 0;
long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ, xAccelAve, yAccelAve, zAccelAve = 0;
double lastAccelX, lastAccelY, lastAccelZ;
float xAccelCheck[100], yAccelCheck[100], zAccelCheck[100];
float minimumActivation = 800.0;
float xAccelBias = 0.8;
float yAccelBias = 0.2;
float zAccelBias = 0.2;

long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ, xRotAve, yRotAve, zRotAve = 0;
double lastRotX, lastRotY, lastRotZ;
float xRotCheck[100], yRotCheck[100], zRotCheck[100];
float xRotBias = 0.0;
float yRotBias = 0.0;
float zRotBias = 0.0;

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

  Wire.begin();
  setupMPU();

}

void loop()
{

  if ( Bluefruit.connected() ) {

    checkForButtonPress();

    if (buttonPressed) {
      Serial.println((millis() - buttonPressedTimer) / 1000);
      delay(100);


      //Takes the accleration data and sets the scaled version
//      if (!hasCalibrated) {
//        calibrate();
//      }

      checkForAction();
    } else {
      velX = 0;
      velY = 0;
      velZ = 0;
      timeBetweenAccelRead = 0;
      lastReadingTime = millis();
    }

  }

}
