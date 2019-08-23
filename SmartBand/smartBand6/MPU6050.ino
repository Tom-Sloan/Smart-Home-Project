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

  printData();

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
  dataString += "\0";
  Serial.print("DATASTRING: ");
  Serial.print(dataString);
  Serial.println("END");
}
