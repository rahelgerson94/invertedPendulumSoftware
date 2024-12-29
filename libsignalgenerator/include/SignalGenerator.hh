#pragma once
//#include "StepperMotor.hh"
#include "Debug.hh"
#include <chrono>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdexcept>
using ofstream = std::ofstream;
using invalid_argument = std::invalid_argument;
using runtime_error = std::runtime_error;

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ofstream;

#define INT(x,y) (std::floor(x/y) + 1)

class SignalGenerator
{
public:
SignalGenerator();
SignalGenerator(float);
float _dt;
vector<float> generateSinusoid(float frequencyHz, 
        float durationSec, 
        float phaseShiftDeg,
        float samplingPeriod);

void generatePulse(vector<float>& inBuf, 
        float periodSec, 
        float durationSec, 
        float dt, 
        float phaseShiftSec);

void generateUnitStepSignal(
        std::vector<float>& unitStepSig, 
        float duration, 
        float startTime,  
        float samplingPeriod);
        
void generateVariableDelayPwm(
        vector<int> &in,
        float duration, //total duration of the signal (s)
        float aRef,
        float delaySec,  
        float tSpan, //# of secs to reach to desired accel, aRef
        float initAngularVelocity,
        float samplingPeriod);
float computeDelayAccel(float prevDelay, 
        float aRef, 
        float tspan, 
        float prevTime,
        float samplingPeriod);
uint computeDelayAccel(
    float aRef, 
    uint N, //number of samples to reach desred 
    uint n); //cur sample number
float computeDelayDeaccel(float prevDelay, 
        float aRef, 
        float tspan, 
        float prevTime,
        float samplingPeriod);
 };
