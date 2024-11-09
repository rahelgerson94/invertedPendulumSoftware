#include "IMU.hh"

IMU::IMU(){}

void IMU::setOffsetsAccel(int16_t x, int16_t y, int16_t z)
{
    MPU6050_Base::setXAccelOffset(x);
    MPU6050_Base::setYAccelOffset(y);
    MPU6050_Base::setZAccelOffset(z);
}
void IMU::getAccel()
{
    int16_t accelRaw[3];
    MPU6050_Base::getAcceleration(&(accelRaw[0]), &(accelRaw[1]), &(accelRaw[2]));
    //convert from raw to float
    for (int i = 0; i < 3; i++)
    {
        this->accel.xyz[i] = accelRaw[i]/ 16384.0;
    }

    //convert to m/s^2
    for (int i = 0; i < 3; i++)
    {
        this->accel.xyz[i] *= 9.81; // m/s^2
    }
}

void IMU::init()
{
    MPU6050_Base::initialize(ACCEL_FS::A2G,  GYRO_FS::G250DPS);
}