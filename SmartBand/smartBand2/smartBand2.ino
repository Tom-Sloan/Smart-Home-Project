
#include <bluefruit.h>

/*
   The bleuar.write() function is amazing and sends the data. Note: If sending as a int it conversts to a string (e.g 77 will be recieved as M)
*/

// BLE Service
BLEDis  bledis; //This is used to get device information
BLEUart bleuart; //This is the important one for communication
BLEBas  blebas; //This is used for the battery

// Button Variables will change:
int buttonState = 0;
int timeDelay = 0;
int timeCounter = 0;
float buttonPressedTimer = 0;
bool buttonPressed = false;

//Accerleration Variabels
int lastX, lastY, lastZ = 0;
float scaledX, scaledY, scaledZ;
int rawX, rawY, rawZ;

//Step counter variables
int stepCounter = 0;
float threshold = 1.00;

// Constants won't change. They're used here to set pin numbers:
const int buttonPin = 27;     // the number of the pushbutton pin
const int ledPin =  LED_BUILTIN;      // the number of the LED pin
const int scale = 3;



void setup()
{
  Serial.begin(115200);
  Serial.println("------------ON---------------\n");

  basicBluetooth();

  // Configure and Start Device Information Service. Don't Touch. Important and must be before bleuart
  bledisStarter();

  // Configure and Start BLE Uart Service
  bleuart.begin();

  // Start BLE Battery Service. Not nessecary. must be started after bleuart
  blebasStarter();

  // Set up and start advertising
  startAdv();

  //Intialiaze all digital pins
  digitalPinStarter();
}

void loop()
{

  //Takes the accleration data and sets the scaled version
  getAccelerationData(rawX, rawY, rawZ);
  //printAccelerationData();
  checkForButtonPress();
  checkForIncomingData();

  if (buttonPressed) {
    checkForAction();
  }
  float totAccel = sqrt(scaledX * scaledX + scaledY * scaledY + scaledZ * scaledZ);
  
  if(totAccel > threshold && millis() - timeCounter > 100){
    char str[16];
    stepCounter += 1;
    sprintf(str, "%d", stepCounter);
    bleuart.write(str);
  }
  
  if (millis() - timeCounter > 100) {
    timeCounter = millis();
    lastX = scaledX;
    lastY = scaledY;
    lastZ = scaledZ;
  }
  
  // Request CPU to enter low-power mode until an event/interrupt occurs
  waitForEvent();
}

//Checks to see if there has been any incoming data
void checkForIncomingData() {
  // Forward from BLEUART to HW Serial
  while ( bleuart.available() )
  {
    uint8_t ch;
    ch = (uint8_t) bleuart.read();
    if (ch == 'a') {
      Serial.print("HIGH");
    } else if (ch == 0) {
      Serial.print("0");
      Serial.print(ch);
    } else if (ch == 1) {
      Serial.print("1");
      Serial.print(ch);
    }
    Serial.write(ch);
  }
}

//Looks to see if any action has been activited
void checkForAction() {
  char str[16];

  if (scaledZ > (lastZ + 1)) {
    Serial.print("Blinds Up\n");
    sprintf(str, "1");
    bleuart.write(str);
  }
  if (scaledZ < (lastZ - 1)) {
    Serial.print("Blinds down\n");
    sprintf(str, "2");
    bleuart.write(str);
  }
  if (scaledX > (lastX + 1)) {
    Serial.print("Lights ON\n");
    sprintf(str, "3");
    bleuart.write(str);
  }
  if (scaledX < (lastX - 1)) {
    Serial.print("Lights Off\n");
    sprintf(str, "4");
    bleuart.write(str);
  }
}





//Sends a string
void sendData(String data) {
  char str[16];
  sprintf(str, "%d", (data));
  bleuart.write(str);
}

//Sends a integer
void sendData(int data) {
  bleuart.write(data);
}







//************************************Stuff I don't use often**********************************************************************************

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
  Serial.print("Y: "); Serial.print(scaledY); Serial.print(" m/s\t");
  Serial.print("Z: "); Serial.print(scaledZ); Serial.println(" m/s");
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
    buttonPressed = true;
    Serial.print("ON BUTTON! ");
    Serial.println(buttonState);
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    buttonPressedTimer = millis();
  }

  if ((millis() - buttonPressedTimer) > 5000) {
    buttonPressed = false;
  }
}

//Called at connection
void connect_callback(uint16_t conn_handle)
{
  char central_name[32] = { 0 };
  Bluefruit.Gap.getPeerName(conn_handle, central_name, sizeof(central_name));

  Serial.print("Connected to ");
  Serial.println(central_name);
}

//Call at disconnection
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  Serial.println();
  Serial.println("Disconnected");
}

//Start advertising bluetooth connection
void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();

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

void bledisStarter() {
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather52");
  bledis.begin();
}
void blebasStarter() {

  blebas.begin();
  blebas.write(100);
}

//Setup the must haves for bluetooth
void basicBluetooth() {
  //unessecary step
  // Setup the BLE LED to be enabled on CONNECT
  // Note: This is actually the default behaviour, but provided
  // here in case you want to control this LED manually via PIN 19
  Bluefruit.autoConnLed(true);

  // Config the peripheral connection with maximum bandwidth
  // more SRAM required by SoftDevice
  // Note: All config***() function must be called before begin()
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

  Bluefruit.begin();
  // Set max power. Accepted values are: -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setTxPower(4);
  Bluefruit.setName("Bluefruit52");
  //Bluefruit.setName(getMcuUniqueID()); // useful testing with multiple central connections
  Bluefruit.setConnectCallback(connect_callback);
  Bluefruit.setDisconnectCallback(disconnect_callback);
}

//Setup pin mode for the digital pins
void digitalPinStarter() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

/* Sends the number of seconds the program has been active
   int a = millis();

  if ((a - timeDelay) > 1000) {
    timeDelay = millis();
    char str[16];
    sprintf(str, "%d", (a / 1000));
    bleuart.write(str);
  }

*/
