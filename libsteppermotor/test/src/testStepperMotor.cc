//#include "StepperMotor.hh"
#include <chrono>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdexcept>
   
float dt = 1.0f / 100.0f;
#define SEC2MILIS 1000.0f
#define MINDELAY dt


using std::cout;
using std::endl;
using std::string;
using std::vector;

vector<float> timeHist;

float computeDelayDeacel(float prevDelay, float aRef, float tspan, float PrevTime);
float computeDelayAccel(float prevDelay, float aRef, float tspan, float PrevTime);

void save2csv(const vector<float>& timeData, int amplitude, const string& path, const string& filename);

int main(void)
{
    float tspan = 100.0f;
    float delay  = 1.0f;
    int numIntervals = 200;
    float curTime = 0.0f+dt;
    float prevDelay = 1.0f; //s
    float aRef = 5; //rad/s^2
    for (int k = 0; k < numIntervals; k++)
    {
        
        if (!(delay <= MINDELAY))
        {
            delay = computeDelayAccel(prevDelay, aRef, tspan,  curTime);
        }
        delay = computeDelayAccel(prevDelay, aRef, tspan,  curTime);
        curTime = curTime + delay;
        prevDelay = delay;
        timeHist.push_back(curTime);
        cout << delay << endl;
        save2csv(timeHist, 1, "data", "test1");
    }

    return 0;
}
/*
compute w[k] = w[k-1] + | (aRef*tspan - w[k-1])  | * (t[k] - t[k-1])
                        |_ (tspan - t[k-1])     _|
    
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

