//Check to see if the button has bee pressed
void checkForAction() {
  if (canSendAgain) {
    if ((millis() - buttonPressedTimer) / 1000 < recordingTime) {
      recordData();
    } else {
      sendData_imu(control);
      canSendAgain = false;
    }
  }
}

//Setup pin mode for the digital pins
void digitalPinStarter() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
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
    delay(100);
    digitalWrite(ledPin, HIGH);
    buttonPressedTimer = millis();
  }

  if ((millis() - buttonPressedTimer) / 1000 > timeBetween) {
    buttonPressed = false;
    canSendAgain = true;
    hasCalibrated = false;
    sendData(hrmc, 0);
    digitalWrite(ledPin, LOW);
  }
}

void startIMU() {
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;

  if (!imu.begin())
  {
    Serial.println("Failed to communicate with LSM9DS1.");
    Serial.println(" -check wiring.");
    Serial.println("Default settings in this sketch will " \
                   "work for an out of the box LSM9DS1 " \
                   "Breakout, but may need to be modified " \
                   "if the board jumpers are.");
    while (1)
      ;
  }
}

//Adds each number of val into data at a specific point i.e -12322 -> [1,1,2,3,2,2], removes the negative by adding a one
int* convert(int data[], int val, int startLoc) {

  if (val < 0) {
    val = val * -1;
    data[startLoc] = 1;
  } else {
    data[startLoc] = 0;
  }
  //Pulls out numbers in reverse order (may decrease by two digits as /100)
  int i = startLoc + 3;
  while (val > 0) {
    int digit = val % 100;
    data[i] = digit;
    val /= 100;
    i--;
  }
  
  for (i; i > startLoc; i--) {
    data[i] = 0;
  }

  return data;
}
//Records
void recordData() {
  readIMU();
  acc_x += imu.ax;
  acc_y += imu.ay;
  acc_z += imu.az;
  gyr_x += imu.gx;
  gyr_y += imu.gy;
  gyr_z += imu.gz;
  mag_x += imu.mx;
  mag_y += imu.my;
  mag_z += imu.mz;
  num_of_rec += 1;
}
void getMeans() {
  imu.ax = acc_x / num_of_rec;
  imu.ay = acc_y / num_of_rec;
  imu.az = acc_z / num_of_rec;
  imu.gx = gyr_x / num_of_rec;
  imu.gy = gyr_y / num_of_rec;
  imu.gz = gyr_z / num_of_rec;
  imu.mx = mag_x / num_of_rec;
  imu.my = mag_y / num_of_rec;
  imu.mz = mag_z / num_of_rec;
  Serial.print("ax: ");
  Serial.print(imu.ax);
  Serial.print(" ay: ");
  Serial.print(imu.ay);
  Serial.print(" az: ");
  Serial.print(imu.az);
  Serial.print(" gx: ");
  Serial.print(imu.gx);
  Serial.print(" gy: ");
  Serial.print(imu.gy);
  Serial.print(" gz: ");
  Serial.print(imu.gz);
  Serial.print(" mz: ");
  Serial.print(imu.mx);
  Serial.print(" my: ");
  Serial.print(imu.my);
  Serial.print(" mz: ");
  Serial.println(imu.mz);

  num_of_rec = 0;
}
