//Note for some of the functions distanceToTravel is used for the amount the motor rotates.
//To test those functions you will have to change distance to travel to not zero


#include <Time.h> //Libaries for time
#include <TimeLib.h>
#include <Stepper.h> // Library for stepper 
#include <bluefruit.h> //Libary for bluetooth
#include <math.h>

//Bluetooth vaiables
BLEUart bleuart;
uint8_t readPacket (BLEUart *ble_uart, uint16_t timeout);// Function prototypes for packetparser.cpp
float   parsefloat (uint8_t *buffer);
void    printHex   (const uint8_t * data, const uint32_t numBytes);
extern uint8_t packetbuffer[];// Packet buffer

//Motor Variables
const int STEPS_PER_REV = 200;
const int SPEED_CONTROL = A0;
int potValue = 0;
int motorSpeed = 0;
int sensorPin = A1;

//Control the position of the blind
int distanceToTravel = 0; //this will be set by the user
int change = 0;//used to change the distanceTravelled
bool canChangeTop = true;
bool canChangeBottom = true;
int top = 0;
int current = 0;
bool dir = true;


//Seting base Time Variables
int timeHours = 0;
int timeMinutes = 0;
int timeDays = 1;
int timeMonths = 8;
int timeYears = 2018;

//Seting the user set up and down times
int riseHours = 5;
int riseMinutes = 0;
int downHours = 21;
int downMinutes = 0;

//For testing to know how much time has passed since start of test
bool canPrint = false;

//Deboucing variables
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

//Instance variable of stepper
Stepper stepper_NEMA17(STEPS_PER_REV, 27, 30, 11, 7);

void setup() {
  //need  this baud rate for bt
  Serial.begin(115200);

  //testing intro
  Serial.println(F("Adafruit Bluefruit52 Controller App Example"));
  Serial.println(F("-------------------------------------------"));

  //Starting the bt
  Bluefruit.begin();
  // Set max power. Accepted values are: -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setTxPower(4);
  Bluefruit.setName("Bluefruit52");

  // Configure and start the BLE Uart service
  bleuart.begin();

  // Set up and start advertising
  startAdv();
}

//Bluetooth advertising method
void startAdv(void)
{

  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include the BLE UART (AKA 'NUS') 128-bit UUID
  Bluefruit.Advertising.addService(bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();

  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
  runMotor(1000, true);// to show its on
  
}

void loop() {
  //checks if time to run
  timeToRun();

  //testing helper if you need to see how much time has passed
  if (canPrint) {

    Serial.print("def sec: ");
    Serial.println(second());
    Serial.print("def min: ");
    Serial.println(minute());
    Serial.print("def hour: ");
    Serial.println(hour());
    Serial.print("set Hour: ");
    Serial.println(downHours);
    Serial.print("set min: ");
    Serial.println(downMinutes);
  }

  // Wait for new data to arrive
  uint8_t len = readPacket(&bleuart, 500);

  if (len == 0) return;

  //Testing information as it displays the info need for the if statements
  Serial.print("Packet buffer ");
  Serial.print(1);
  Serial.print(": ");
  Serial.write(packetbuffer[1]);
  Serial.println("");

  // Got a packet!
  printHex(packetbuffer, len);

  // Run motor by sending a packet with 2143
  if (packetbuffer[1] == 'C') {
    if(dir){
      runMotor(1000, true);
      dir = !dir;
    }else if(!dir){
      runMotor(1000, false);
      dir = !dir;
    }
  }
  
  if (packetbuffer[1] == 'D') {
    if (packetbuffer[2] <= 2 && packetbuffer[3] < 10 && packetbuffer[4] < 6 && packetbuffer[5] <= 9) {
      if ((packetbuffer[2] < 2) || (packetbuffer[2] == 2 && packetbuffer[3] < 5)) {
        if (packetbuffer[6] == 1) {
          downMinutes = packetbuffer[4] * 10 + packetbuffer[5];
          downHours = packetbuffer[2] * 10 + packetbuffer[3];

          Serial.print("downHours:");
          Serial.println(downHours);
          Serial.print("downMinutes: ");
          Serial.println(downMinutes);

        } else if (packetbuffer[6] == 0)
        {
          riseMinutes = packetbuffer[4] * 10 + packetbuffer[5];
          riseHours = packetbuffer[2] * 10 + packetbuffer[3];

          Serial.print("riseHours:");
          Serial.println(riseHours);
          Serial.print("riseMinutes: ");
          Serial.println(riseMinutes);
        }
      }

    }
  }

  if (packetbuffer[1] == 'T') {
    //starts the tests prints at start of the loop
    canPrint = true;

    //Setting all the times to the values given in the packet
    timeMinutes = packetbuffer[4] * 10 + packetbuffer[5];
    timeHours = packetbuffer[2] * 10 + packetbuffer[3];
    timeDays = packetbuffer[6] * 10 + packetbuffer[7];
    timeMonths = packetbuffer[8] * 10 + packetbuffer[9];
    timeYears = packetbuffer[10] * 10 + packetbuffer[11] + 2000;

    //setting the base time, this only has to be run once and has to be run first. The method takes care of all the error handling
    setTime(timeHours, timeMinutes, 0, timeDays, timeMonths, timeYears );

    //Printing for tests
    Serial.print("Hours:");
    Serial.println(hour());
    Serial.print("Minutes: ");
    Serial.println(minute());
    Serial.print("Days:");
    Serial.println(day());
    Serial.print("Months: ");
    Serial.println(month());
    Serial.print("Year:");
    Serial.println(year());
  }

  //Move a distance
  if (packetbuffer[1] == 'U') {
    if (packetbuffer[3] == 'B') { //go backwards
      if (packetbuffer[2] == 'S') {
        runMotor(500, false);
        changeDistance(-500);
      } else if (packetbuffer[2] == 'T') {
        runMotor(5000, false);
        changeDistance(-5000);
      }
    } else if (packetbuffer[3] == 'F') { //go backwards
      if (packetbuffer[2] == 'S') {
        runMotor(500, true);
        changeDistance(500);
      } else if (packetbuffer[2] == 'T') {
        runMotor(5000, true);
        changeDistance(5000);
      }
    }
  }

  //Set max travel distance to current amount 215455 (top)//215442 (bottom)
  if (packetbuffer[1] == 'S') {
    //check for at the top
    if (packetbuffer[2] == 'T') {
      canChangeTop = false;
    } else if (packetbuffer[2] = 'B') { //check for at the bottom
      canChangeBottom = false;
    }
  }

  //allows the max distance to be reset 2153
  if (packetbuffer[1] == 'R') {
    canChangeTop = true;
    canChangeBottom = true;
  }
}

//Runs when time comes
void timeToRun() {
  //Method to run when the set time comes
  if (hour() == riseHours && minute() == riseMinutes && second() == 0) {
    Serial.print("rise Hour: ");
    Serial.print(riseHours);
    Serial.print("rise min: ");
    Serial.print(riseMinutes);
    Serial.print("def Hour: ");
    Serial.print(hour());
    Serial.print("def min: ");
    Serial.print(minute());

    runMotor(distanceToTravel, true);

  }
  if (hour() == downHours && minute() == downMinutes && second() == 0) {
    Serial.print("set Hour: ");
    Serial.print(downHours);
    Serial.print("set min: ");
    Serial.print(downMinutes);
    Serial.print("def Hour: ");
    Serial.print(hour());
    Serial.print("def min: ");
    Serial.print(minute());

    runMotor((distanceToTravel-current), false);
  }
}

//sets the distanceToTravel
void changeDistance(int change) {
  if(!canChangeTop && canChangeBottom){
    distanceToTravel += change;
  }else if(!canChangeBottom && canChangeTop){
    distanceToTravel += change;
  }else if(!canChangeBottom && !canChangeTop){
    distanceToTravel = -abs(distanceToTravel);
  }
}

//Runs motor at the set potentiometer speed
void runMotor(int distance, bool dir) {

  if (dir) {//Controls direction
    if (canChangeBottom) {//if the limit is not set let it move
      Serial.println("Motor Running forwards (Blind Lowering)");
      lastDebounceTime = millis(); // For deboucing
      stepper_NEMA17.setSpeed(100);
      stepper_NEMA17.step(distance);
      
    } else if ((distance + current) >= distanceToTravel && !canChangeBottom) { //When the limit is set
      Serial.println("Motor Running forwards (Blind Lowering)");
      lastDebounceTime = millis(); // For deboucing
      stepper_NEMA17.setSpeed(100);
      stepper_NEMA17.step(distance);
      current += distance;
      
    } else {
      Serial.print("Sorry your too close to the end to do");
    }
  } else if (!dir) {
    if (canChangeTop) {//if the limit is not set let it move
      Serial.println("Motor Running forwards (Blind Raising)");
      lastDebounceTime = millis(); // For deboucing
      stepper_NEMA17.setSpeed(100);
      stepper_NEMA17.step(-distance);
      
    } else if ((-distance + current) >= distanceToTravel && !canChangeTop) {//When the limit is set
      Serial.println("Motor Running forwards (Blind Raising)");
      lastDebounceTime = millis(); // For deboucing
      stepper_NEMA17.setSpeed(100);
      stepper_NEMA17.step(-distance);
      current -= distance;
      
    } else {
      Serial.print("Sorry your too close to the end to do");
    }
  }
}






