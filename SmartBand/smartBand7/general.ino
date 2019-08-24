

void startIMU() {
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;

  if (!imu.begin())
  {
    outputln("Failed to communicate with LSM9DS1.");
    outputln(" -check wiring.");
    outputln("Default settings in this sketch will " \
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
  //Serial.print("ax: ");
  Serial.print(imu.ax);
  Serial.print(",");
  Serial.print(imu.ay);
  Serial.print(",");
  Serial.print(imu.az);
  Serial.print(",");
  Serial.print(imu.gx);
  Serial.print(",");
  Serial.print(imu.gy);
  Serial.print(",");
  Serial.print(imu.gz);
  Serial.print(",");
  Serial.print(imu.mx);
  Serial.print(",");
  Serial.print(imu.my);
  Serial.print(",");
  Serial.println(imu.mz);

  num_of_rec = 0;
}

void output(String txt){
  if(canOutput){
    Serial.print(txt);
  }
}
void output(int txt){
  if(canOutput){
    Serial.print(txt);
  }
}
void outputln(String txt){
  if(canOutput){
    Serial.println(txt);
  }
}
