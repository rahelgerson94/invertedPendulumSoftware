#pragma once
#include "Sensor.hh"
#include "MPU6050.h"
#include "I2Cdev.h"

class Acceleremoter: public Sensor
{
    public:
    
    void* wireObj;
    Acceleremoter(){}
    Acceleremoter(uint8_t deviceAddress): Sensor(){
        this->deviceAddress = deviceAddress;
    }
    
    void read(uint8_t regAddress, int16_t* x, int16_t* y, int16_t* z)
    {
       I2Cdev::readBytes(this->deviceAddress, regAddress, 6, buffer,  I2Cdev::readTimeout, wireObj);
        *x = (((int16_t)buffer[0]) << 8) | buffer[1];
        *y = (((int16_t)buffer[2]) << 8) | buffer[3];
        *z = (((int16_t)buffer[4]) << 8) | buffer[5];

        this->x = ((*x/16384.0)+xOffset )* 9.81;
        this->y = ((*y/16384.0)+yOffset) * 9.81;
        this->z = (*z/16384.0) * 9.81;
    }


    protected:
    uint8_t buffer[14];
};

class Gyro: public Sensor
{
    public:
    Gyro(): Sensor( ){}
};

class IMU: public MPU6050_Base, public Sensor
{
    public:
    Acceleremoter accel;
    IMU();
    void getAccel();
    void init();
    void setOffsetsAccel(int16_t x,int16_t y,int16_t z);
};