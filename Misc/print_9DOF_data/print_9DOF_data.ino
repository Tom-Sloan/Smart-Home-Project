#include <SPI.h>
#include <SparkFunLSM9DS1.h>
LSM9DS1 imu;
#define LSM9DS1_M  0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW
#define DECLINATION -13.01 // Declination (degrees) in Boulder, CO.

void setup() {
  Serial.begin(115200);
  
  startIMU();

  Serial.println("Starting");
}

void loop() {
 
 readIMU();
 printData();

}
void startIMU() {
  Serial.println("Starting imu");
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
  Serial.println("Sone Imu");
}
void printData() {
  String dataString = "";
  dataString += imu.ax;
  dataString += ",";
  dataString += imu.ay;
  dataString += ",";
  dataString += imu.az;
  dataString += ",";
  dataString += imu.gx;
  dataString += ",";
  dataString += imu.gy;
  dataString += ",";
  dataString += imu.gz;
  dataString += ",";
  dataString += imu.mx;
  dataString += ",";
  dataString += imu.my;
  dataString += ",";
  dataString += imu.mz;
  Serial.println(dataString);
}

 void readIMU() {

  if ( imu.gyroAvailable() )
  {
    // To read from the gyroscope,  first call the
    // readGyro() function. When it exits, it'll update the
    // gx, gy, and gz variables with the most current data.
    imu.readGyro();
  }
  if ( imu.accelAvailable() )
  {
    // To read from the accelerometer, first call the
    // readAccel() function. When it exits, it'll update the
    // ax, ay, and az variables with the most current data.
    imu.readAccel();
  }
  if ( imu.magAvailable() )
  {
    // To read from the magnetometer, first call the
    // readMag() function. When it exits, it'll update the
    // mx, my, and mz variables with the most current data.
    imu.readMag();
  }

}
