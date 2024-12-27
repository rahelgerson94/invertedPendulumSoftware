#include "StepperMotor.hh"

//#include "StepperMotor.hh"
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

float dt = 1.0f / 1000.0f;
const float mu = 0.55; // friction coefficient
// Variables for calculation
const float l = 0.0254; // m
const float m = 0.065; // kg
const float M = 0.33; // kg
const float r = 0.0125; // m, pulley radius attached to the motor

#define SEC2MILIS 1000.0f
#define MINDELAY dt

vector<float> timeHist;
StepperMotor stepper;
float computeDelayDeacel(float prevDelay, float aRef, float tspan, float PrevTime);
float computeDelayAccel(float prevDelay, float aRef, float tspan, float PrevTime);
void save2csv(const vector<float>& timeData, int amplitude, const string& path, const string& filename);

void save2csv2(const vector<float>& timeData, const vector<float>& ampData, const string& path, const string& filename);
vector<float>  generateSinusoid(float frequencyHz, float durationSec, float dt, float phaseShiftDeg);
void generatePulse(vector<float>& inBuf, float periodSec, float durationSec, float dt, float phaseShiftSec);
vector<float> Va;
vector<float> Vb;
vector<float> in1;
vector<float> in2;
void generateUnitStepSignal(std::vector<float>& unitStepSig, float duration, float startTime);
int main(void)
{
   cout << "entered main" << endl;
    float duration = 7.0f;
    float delay  = 1.0f;
    int numIntervals = 200;
    float curTime = 0.0f+dt;
    float prevDelay = delay; //s
    float aRef = 5; //rad/s^2
    uint n = std::floor(duration/dt) + 1;
    cout << "n = " << n << endl;
    Vb.resize(n); in1.resize(n);
    Va.resize(n); in2.resize(n);
    generatePulse(Va, 4, 2, dt, 0);
    generatePulse(Vb, 4, 2, dt, 2/2);
    timeHist.resize(n);
    std::vector<float> zeros(n, 0.0f);
    for (int i = 0; i < n; i ++)
    {
        timeHist[i] = i*dt;
    }
    
    generateUnitStepSignal(in1, 1.0f, 0);
    save2csv2(timeHist, Vb, "data", "Vb");
    save2csv2(timeHist, Va, "data", "Va");
    save2csv2(timeHist, in1, "data", "unitStep");
    vector<float> inputs(2);
    float loadTorque =mu*(M+m)*g*r;
    for (int i = 0; i < n; i ++)
    {
        
        inputs[0] = Va[i];
        inputs[1] = Vb[i];
        
        stepper.updateStates(inputs, loadTorque, dt);
        stepper.storeData();
    }
    stepper.save("data/stepperNominalNoLoadTorqueInertiaBigger");
    return 0;
}
/*
compute w[k] = w[k-1] + | (aRef*tspan - w[k-1])  | * (t[k] - t[k-1])
                        | (tspan - t[k-1])     _|
    
    w[k]: cur angular vel
    w[k-1]: prev angular vel
    tspan: the time span in which to reach the desried acceleration
    t[k]: cur time
    t[k-1]: time in prev timesteo

    note, the delay is given by :
        delay = 1/w[k]
*/
float computeDelayDeacel(float prevDelay, float aRef, float tspan, float prevTime)
{
    float wPrev = (1/prevDelay);
    float wCur = wPrev + ((aRef*tspan - wPrev)/(tspan - (prevTime)))*(dt);
    return 1/wCur;
}

float computeDelayAccel(float prevDelay, float aRef, float tspan, float prevTime)
{
    float wPrev = (1/prevDelay);
    float wCur = wPrev + ((aRef*tspan - wPrev)/(tspan - (prevTime)))*(dt);
    return 1/wCur;
}
// Function to save the output to a CSV file
void save2csv(const vector<float>& timeData, int amplitude, const string& path, const string& filename)
{
    // Construct the full file path
    string fullPath = path + "/" + filename;

    // Open the file for writing
    std::ofstream outFile(fullPath);
    if (!outFile.is_open())
    {
        throw std::runtime_error("Unable to open file for writing: " + fullPath);
    }

    // Write the header
    outFile << "time,amplitude\n";

    // Write the data
    for (const float& time : timeData)
    {
        outFile << time << "," << amplitude << "\n";
    }

    // Close the file
    outFile.close();
}


vector<float> generateSinusoid(float frequency, float duration, float dt, float phaseShiftDeg) {
    float omega = 2 * M_PI * frequency; // Angular frequency (omega = 2 * pi * frequency)
    float phi = phaseShiftDeg*(M_PI/180);
    vector<float> sineWave;
    
    for (float t = 0; t < duration; t = t + dt) {
        
        sineWave.push_back(std::sin((omega * t))); // Calculate sin(w * t)
    }

    return sineWave;
}


void generatePulse(vector<float>& pulseTrain, // Store the pulse values (0 or 1)
float periodSec, float durationSec, float dt, float phaseShiftSec) {

    uint n = std::floor(durationSec/dt) + 1;
    for (int i = 0; i < n; i++) {
        // Apply phase shift
        float t = i * dt; // Calculate time from index
        float shiftedTime = t + phaseShiftSec; // Apply phase shift


        // Determine if we are in the high state (1) or low state (0) of the pulse
        if (fmod(shiftedTime, periodSec) < periodSec / 2) {
            pulseTrain[i] = 1.0f; // High state
        } else {
            pulseTrain[i] = 0.0f;  // Low state
        }
    }
}


void save2csv2(const vector<float>& timeData, const vector<float>& ampData, const string& path, const string& filename) {
    if (timeData.size() != ampData.size()) {
        throw std::invalid_argument("timeData and ampData must have the same size.");
    }

    string fullPath = path + "/" + filename;
    ofstream file(fullPath);

    if (!file.is_open()) {
        throw runtime_error("Failed to open file: " + fullPath);
    }
    file << "time,amplitude\n";
    // Write data in the format t, y(t)
    for (size_t i = 0; i < timeData.size(); ++i) {
        file << timeData[i] << ", " << ampData[i] << "\n";
    }

    file.close();
    cout << "Data saved to " << fullPath << endl;
}
#include <vector>

void generateUnitStepSignal(std::vector<float>& unitStepSig, float duration, float startTime) {
    size_t totalSize = unitStepSig.size();  // Total number of samples
    float dt = 1.0f / totalSize;           // Time resolution (assuming normalized range [0, 1])

    // Calculate the indices for start and end times
    size_t startIndex = (startTime / dt >= 0) ? static_cast<size_t>(startTime / dt) : 0;
    size_t endIndex = (startTime + duration) / dt <= totalSize ? static_cast<size_t>((startTime + duration) / dt) : totalSize;

    // Write ones from startIndex to endIndex
    for (size_t i = startIndex; i < endIndex; ++i) {
        unitStepSig[i] = 1.0f;
    }
}
