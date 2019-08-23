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

BLEClientService        hrms(UUID16_SVC_HEART_RATE);
BLEClientCharacteristic hrmc(UUID16_CHR_HEART_RATE_MEASUREMENT);
BLEClientCharacteristic bslc(UUID16_CHR_BODY_SENSOR_LOCATION);

BLEClientService        blindUUID(0x7777);
BLEClientService        lightUUID(0x8888);

BLEClientService* temp;
BLEClientCharacteristic toggleCUUID(0x0001);
BLEClientCharacteristic responseCUUID(0x0002);
BLEClientCharacteristic textCUUID(0x0003);

int lightPin = 27;
int blindPin = 30;

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

  // Initialize HRM client
  //hrms.begin();

  // Initialize client characteristics of HRM.
  // Note: Client Char will be added to the last service that is begin()ed.
  // bslc.begin();

  // set up callback for receiving measurement
  //  hrmc.setNotifyCallback(hrm_notify_callback);
  //  hrmc.begin();

  blindUUID.begin();

  responseCUUID.begin();

  // Increase Blink rate to different from PrPh advertising mode
  Bluefruit.setConnLedInterval(250);

  // Callbacks for Central
  Bluefruit.Central.setDisconnectCallback(disconnect_callback);
  Bluefruit.Central.setConnectCallback(connect_callback);

  pinMode(lightPin, INPUT);
  pinMode(blindPin, INPUT);

  connectToDevice(blindUUID);
}

void loop()
{
  //  if (lightPin == HIGH) {
  //    connectToDevice(blindUUID);
  //  } else if (blindPin == HIGH) {
  //    connectToDevice(blindUUID);
  //  } else {
  //    connectToDevice(hrms);
  //  }
  if ( !Bluefruit.connected() ) {

    
  }

}

void connectToDevice(BLEClientService ble) {
  /* Start Central Scanning
     - Enable auto scan if disconnected
     - Interval = 100 ms, window = 80 ms
     - Don't use active scan
     - Filter only accept HRM service
     - Start(timeout) with timeout = 0 will scan forever (until connected)
  */
  Serial.print("Blind");

  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
  Bluefruit.Scanner.filterUuid(ble.uuid);
  Bluefruit.Scanner.useActiveScan(false);
  Bluefruit.Scanner.start(0);                   // // 0 = Don't stop scanning after n seconds
  temp = &ble;


}
/**
   Callback invoked when scanner pick up an advertising data
   @param report Structural advertising data
*/
void scan_callback(ble_gap_evt_adv_report_t* report)
{
  // Since we configure the scanner with filterUuid()
  // Scan callback only invoked for device with hrm service advertised
  // Connect to device with HRM service in advertising
  Bluefruit.Central.connect(report);
}

/**
   Callback invoked when an connection is established
   @param conn_handle
*/
void connect_callback(uint16_t conn_handle)
{
  Serial.println("Connected");
  Serial.print("Discovering HRM Service ... ");

  // If HRM is not found, disconnect and return
  if ( !(*temp).discover(conn_handle))
  {
    Serial.println("Found NONE");
    // disconect since we couldn't find HRM service
    Bluefruit.Central.disconnect(conn_handle);
    return;
  }


  Serial.println("Found it");

  //Bluefruit.Central.disconnect(conn_handle);


  //  if ( !toggleCUUID.discover() )
  //  {
  //    Serial.println("Found NONE");
  //
  //  } else
  //  {
  //
  //    Serial.println("Found it");
  //    toggleCUUID.write8(2);
  //  }

  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if ( responseCUUID.discover() )
  {
    Serial.println("Readent value");
  } else
  {
    Serial.println("Coul d troubleshooting");
  }
}

/**
   Callback invoked when a connection is dropped
   @param conn_handle
   @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
*/
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  Serial.println("Disconnected");
}


/**
   Hooked callback that triggered when a measurement value is sent from peripheral
   @param chr   Pointer client characteristic that even occurred,
                in this example it should be hrmc
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
