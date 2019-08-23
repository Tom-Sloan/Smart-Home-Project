
//sends data in normal method
void sendData(BLECharacteristic BLEChar, int data) {
  //first value is the flag value. I can be set to legnth of array - 2 (therefore it could also be 0 currently) or that value(only tested for length 2)
  uint8_t dataArr[2] = { 0b00000110, data };           // Sensor connected, increment BPS value
  Serial.println(data);
  // Note: We use .notify instead of .write!
  // If it is connected but CCCD is not enabled
  // The characteristic's value is still updated although notification is not sent

  if (BLEChar.notify(dataArr, sizeof(dataArr))) {
    //Serial.print("Heart Rate Measurement updated to: "); Serial.println(data);

  } else {
    //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
  }
}

//Sends the 36 bits 
void sendData_imu(BLECharacteristic BLEChar) {
  //first value is the flag value. I can be set to legnth of array - 2 (therefore it could also be 0 currently) or that value(only tested for length 2)
  // Sensor connected, increment BPS valu
  // Note: We use .notify instead of .write!
  // If it is connected but CCCD is not enabled
  // The characteristic's value is still updated although notification is not sent

  Serial.println("SENDING: ");
  getMeans();
  
  convert(val, imu.ax, 0);
  convert(val, imu.ay, 4);
  convert(val, imu.az, 8);
  convert(val, imu.gx, 12);
  convert(val, imu.gy, 16);
  convert(val, imu.gz, 20);
  convert(val, imu.mx, 24);
  convert(val, imu.my, 28);
  convert(val, imu.mz, 32); 
  for(int i = 0; i < 36; i++){
    Serial.print(val[i]);
  }
  Serial.println();
  Serial.print(num_of_rec);
  Serial.print("CONT");
  uint8_t dataArr[36] = { val[0], val[1], val[2], val[3], val[4], val[5], val[6], val[7], val[8], val[9], val[10], val[11], val[12], val[13], val[14], val[15], val[16], val[17], val[18], val[19], val[20], val[21], val[22], val[23], val[24], val[25], val[26] ,val[27], val[28], val[29], val[30], val[31], val[32], val[33], val[34], val[35]};
  if (BLEChar.notify(dataArr, sizeof(dataArr))) {
    //Serial.print("Heart Rate Measurement updated to: "); Serial.println(data);

  } else {
    //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
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

  control.setProperties(CHR_PROPS_NOTIFY);
  control.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  control.setFixedLen(36);// Change this number to chnage length of message
  control.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  control.begin();


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
  
  if (chr.uuid == control.uuid) {
    if (chr.notifyEnabled()) {
      Serial.println("Control 'Notify' enabled");
    } else {
      Serial.println("Control 'Notify' disabled");
    }
  }
}
