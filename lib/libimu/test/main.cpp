/*
  MPU6050 Raw

  A code for obtaining raw data from the MPU6050 module with the option to
  modify the data output format.

  Find the full MPU6050 library documentation here:
  https://github.com/ElectronicCats/mpu6050/wiki
*/
#include "I2Cdev.h"
#include "MPU6050.h"
#include "IMU.hh"

/* MPU6050 default I2C address is 0x68*/
IMU imu;
//MPU6050 mpu(0x69);         //Use for AD0 high
//MPU6050 mpu(0x68, &Wire1); //Use for AD0 low, but 2nd Wire (TWI/I2C) object.

/* OUTPUT FORMAT DEFINITION----------------------------------------------------------------------------------
- Use "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated list of the accel 
X/Y/Z and gyro X/Y/Z values in decimal. Easy to read, but not so easy to parse, and slower over UART.

- Use "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit binary, one right after the other. 
As fast as possible without compression or data loss, easy to parse, but impossible to read for a human. 
This output format is used as an output.
--------------------------------------------------------------------------------------------------------------*/ 
#define OUTPUT_READABLE_ACCELGYRO

int16_t accelRaw[3];
bool blinkState;
char axes[3] = {'x', 'y', 'z'};
void setup() {
  /*--Start I2C interface--*/
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin(); 
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif

  Serial.begin(9600); //Initializate Serial wo work well at 8MHz/16MHz
  
  /*Initialize device and check connection*/ 
  Serial.println("Initializing MPU...");
  //mpu.initialize(ACCEL_FS::A2G,  GYRO_FS::G250DPS);
  imu.init();

  Serial.println("Testing MPU6050 connection...");
  /* Use the code below to change accel/gyro offset values. Use MPU6050_Zero to obtain the recommended offsets */ 
  Serial.println("Updating internal sensor offsets...\n");
  imu.setOffsetsAccel(-709, -3900, 1230);
  /*Print the defined offsets*/
  Serial.print(imu.getXAccelOffset());
  Serial.print("\t");
  Serial.print(imu.getYAccelOffset()); 
  Serial.print("\t");
  Serial.print(imu.getZAccelOffset());
  Serial.println();

}

void loop() {
  /* Read raw accel/gyro data from the module. Other methods commented*/
  // mpu.getMotion6(&(accelRaw[0]), &(accelRaw[1]), &(accelRaw[2]),
  // &(gyroRaw[0]), &(gyroRaw[1]), &(gyroRaw[2]) );

  imu.getAccel();
  for (int i = 0; i < 3; i++)
  {
    Serial.print(imu.accel.xyz[i]); Serial.print("\t");
  }
  Serial.println();
}
