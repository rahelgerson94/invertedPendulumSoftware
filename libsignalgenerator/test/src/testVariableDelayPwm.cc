#include "SignalGenerator.hh"
#include <chrono>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdexcept>
   
float dt = 1.0f / 5.0f;
#define SEC2MILIS 1000.0f
#define MINDELAY dt


using std::cout;
using std::endl;
using std::string;
using std::vector;

vector<int> varDelayPwmSig;
void save2csv(const vector<float>& timeData, int amplitude, const string& path, const string& filename);
void save2csv2(const vector<float>& timeData, const vector<int>& ampData, const string& path, const string& filename);

SignalGenerator sigGen;
int main(void)
{
    float tSpan = 40.0f;
    float duration = 100.0f; 
    float aRef = 5.0f;
    float delaySec = 0.0f; 
    uint totalSamaplesPerSigDuration = (INT(duration, dt));
    float initAngularVelocity = 1.0f;
    vector<float> timeHist;
    timeHist.resize(totalSamaplesPerSigDuration);
    
    for (int n = 0; n <= totalSamaplesPerSigDuration; n ++)
    {
        timeHist[n] = n*dt;
    }
    cout << "timeHist.size = " << timeHist.size()<< endl;
    sigGen.generateVariableDelayPwm(
        varDelayPwmSig,
         duration, //total duration of the signal (s)
         aRef,
         delaySec,  
         tSpan, //# of secs to reach to desired accel, aRef
         initAngularVelocity,
         dt);
    cout << "varDelayPwmSig.size = " << varDelayPwmSig.size() << endl;
    save2csv2(timeHist, varDelayPwmSig,  "data", "varDelayPwmSig");

    return 0;
}


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


void save2csv2(const vector<float>& timeData, const vector<int>& ampData, const string& path, const string& filename) {
    if (timeData.size() != ampData.size()) {
        throw std::invalid_argument("timeData and ampData must have the same size.");
    }
    DB_ENTER();
    string fullPath = path + "/" + filename;
    DB_PRINT("fullPath: %s", fullPath.c_str());
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
