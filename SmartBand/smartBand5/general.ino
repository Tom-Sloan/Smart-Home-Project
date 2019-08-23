
void checkForAction() {
  if (canSendAgain) {
    updateCheck();

    xAccelAve = abs(average(xAccelCheck, 100) - lastAccelX);
    yAccelAve = abs(average(yAccelCheck, 100) - lastAccelY);
    zAccelAve = abs(average(zAccelCheck, 100) - lastAccelZ);
    xRotAve = abs(average(xRotCheck, 100) - lastRotX);
    yRotAve = abs(average(yRotCheck, 100) - lastRotY);
    zRotAve = abs(average(zRotCheck, 100) - lastRotZ);

    //    Serial.print("xAccelAve: ");
    //    Serial.print(xAccelAve);
    //    Serial.print("\t yAccelAve: ");
    //    Serial.print(yAccelAve);
    //    Serial.print("\t zAccelAve: ");
    //    Serial.print(zAccelAve);
    //    Serial.print("\t xAccelAve: ");
    //    Serial.print(xRotAve);
    //    Serial.print("\t yAccelAve: ");
    //    Serial.print(yRotAve);
    //    Serial.print("\t zAccelAve: ");
    //    Serial.println(zRotAve);

    // if y/x accel yrot cant be biggest zaccel->has yrot
    // if z/x accel zrot cant be biggest yaccel->has zrot/(very small xrot)


    if ( velX > velY && velX > velZ && velX > minimumActivation ) {
      Serial.print("Blinds Up\n");
      blindStatus = 1;
      sendData(blind, blindStatus);
      canSendAgain = false;
    } else if (velY > velX && velY > velZ && velY > minimumActivation ) {
      Serial.print("Lights Off\n");
      Serial.println(yAccelAve);
      lightStatus = 4;
      sendData(light, lightStatus);
      canSendAgain = false;
    } else if (velZ > 350) {
      Serial.print("Lights ON\n");
      Serial.println(yAccelAve);
      lightStatus = 3;
      sendData(light, lightStatus);
      canSendAgain = false;
    }
    delay(5);
  }
}

void updateCheck() {

  recordAccelRegisters();
  recordGyroRegisters();
  for (int i = 99; i > 0; i--) {

    xAccelCheck[i - 1] = xAccelCheck[i];
    yAccelCheck[i - 1] = yAccelCheck[i];
    zAccelCheck[i - 1] = zAccelCheck[i];

    xRotCheck[i - 1] = xRotCheck[i];
    yRotCheck[i - 1] = yRotCheck[i];
    zRotCheck[i - 1] = zRotCheck[i];

  }
  xAccelCheck[99] = gForceX;
  yAccelCheck[99] = gForceY;
  zAccelCheck[99] = gForceZ;

  xRotCheck[99] = rotX;
  yRotCheck[99] = rotY;
  zRotCheck[99] = rotZ;

}

double average(float * arr, int len) {
  //Serial.println("Calc Average");
  double total = 0.0;
  for (int i = 0; i < 100; i++) {
    total += arr[i];
  }
  return total / len;
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

void calibrate() {
  Serial.println("Calibrating");
  for (int i = 0; i < 100; i++) {
    if (i == 0) {
      timeBetweenAccelRead = 0;
      lastReadingTime = millis();
    }
    recordAccelRegisters();
    recordGyroRegisters();

    xAccelCheck[i] = gForceX;
    yAccelCheck[i] = gForceY;
    zAccelCheck[i] = gForceZ;

    lastAccelX += gForceX;
    lastAccelY += gForceY;
    lastAccelZ += gForceZ;

    xRotCheck[i] = rotX;
    yRotCheck[i] = rotY;
    zRotCheck[i] = rotZ;

    lastRotX += rotX;
    lastRotY += rotY;
    lastRotZ += rotZ;
  }

  lastRotX = lastRotX / 100;
  lastRotY = lastRotY / 100;
  lastRotZ = lastRotZ / 100;
  lastAccelX = lastAccelX / 100;
  lastAccelY = lastAccelY / 100;
  lastAccelZ = lastAccelZ / 100;
  Serial.println("OUT");
  velX = 0;
  velY = 0;
  velZ = 0;
  hasCalibrated = true;
}
