#include "SignalGenerator.hh"

SignalGenerator::SignalGenerator(float samplingPeriod):_dt(samplingPeriod){}
SignalGenerator::SignalGenerator(){}
float SignalGenerator::computeDelayDeaccel(
    float prevDelay, 
    float aRef, 
    float tspan, 
    float prevTime, 
    float samplingPeriod)
{
    float dt = samplingPeriod;
    float wPrev = (1/prevDelay);
    float wCur = wPrev + ((aRef*tspan - wPrev)/(tspan - (prevTime)))*(dt);
    return 1/wCur;
}
/*
compute w[k] =  | (aRef*tspan - w[n-1])  | * n
                | (N - (n-1))           _|
    
    w[n]:  angular vel @ cur sample
    w[n-1]:  angular vel @ prev sample
    N: total number of samples needed to reach desried acceleration.
        N = tspan / dt
    n: cur sample
    n-1: prev sample

    note, the delay is given by :
        delay = 1/w[n]
*/
float SignalGenerator::computeDelayAccel(
    float prevDelay, 
    float aRef, 
    float tspan, 
    float prevTime,
    float samplingPeriod)
{
    float dt = samplingPeriod;
    float wPrev = (1/prevDelay);
    float wCur = wPrev + ((aRef*tspan - wPrev)/(tspan - (prevTime)))*(dt);
    return 1/wCur;
}


uint SignalGenerator::computeDelayAccel(
    float aRef, 
    uint N, //number of samples to reach desred 
    uint n) //cur sample number
    
{
    float wCur = (static_cast<float>(n) / static_cast<float>(N)) * aRef;

    return INT(1.0f, wCur);
}

/*
- float duration: total duration of the signal (s)
- float aRef,
- float phaseShiftSec,  
- float tSpan: # of secs to reach to desired accel, aRef
- float numIntervals: divide tspan by numIntervals 
    more intervals means finer resolution
     & smoother ramp up to aRef
- float initAngularVelocity,
- float samplingPeriod)
*/
// void SignalGenerator::generateVariableDelayPwm(
//         vector<int> &in,
//         float duration, //total duration of the signal (s)
//         float aRef,
//         float phaseShiftSec,  
//         float tSpan, //# of secs to reach to desired accel, aRef
//         float numIntervals, //how finely to 
//         float initAngularVelocity,
//         float samplingPeriod)
// { 
//     cout << "entered [generateVariableDelayPwm]... " << endl;
//     float dt = samplingPeriod;
//     float minDelay = dt;
//     uint N = floor(duration/dt);
//     cout << "\tN is: " << N << endl; 
//     float aCur = 0; 
//     float delay = 1.0f/initAngularVelocity;
//     float prevDelay = 1.0f/initAngularVelocity;
//     uint n; //number of samples per current computed delay
//     uint i = 0;
//     uint j=0;
//     while (i < N)
//     {
//         if ((aCur < aRef) || (delay > minDelay)) // 1 1
//         {
//             //nominal 
//             delay = computeDelayAccel(prevDelay, aRef, tSpan,  i*dt, dt);
//             n = std::floor(delay/dt) + 1;
//             if (j < n)
//             {
//                 in.push_back(1);
//                 i++; 
//                 j++;
//             }
//             if (j < 2*n)
//             {
//                 in.push_back(0);
//                 i++;
//                 j++;
//             }
//             j = 0;
//             //update vars
//             prevDelay = delay;
//         }//end nominal case, delay > minDelay && aCur < aRef
//         else
//         /*
//         if any one of these are treu, want future signal
//         fequeny to be constnnt. so we need to break out of this loop
//         and save the delay and samps per delay
//         */
//         {
//             /* have reached desried acce with 
//             the smalledt delay system allows.
//             */
//             if ((delay <= minDelay) || (aCur >= aRef) ) // 0 0 
//             {
//                 delay  = minDelay;
//                 n = std::floor(delay/dt) + 1;
//                 break;
//             }
//             /* have not reached desried accel, but are unable 
//             to redcue the delay further, so desired accel unatainable
//             */
//             else if ((delay <= minDelay) || (aCur < aRef) ) //0 1
//             {
//                 delay  = minDelay;
//                 n = std::floor(delay/dt) + 1;
//                 cout << "warning: cannot produce an accel of "
//                 << aRef <<  "because maxFrequency has already ben reached" << endl;
//                 break;
//             }

//             /* reached desried accel, no longer need to decrease the delay*/
//             else if ((delay > minDelay) || (aCur >= aRef) ) //1 0 
//             {
//                 delay = delay;
//                 n = std::floor(delay/dt) + 1;
//                 break;
//             }
//         } //end else (non-nominal cases)
//     } //end while
    

    
//     n = std::floor(delay/dt) + 1;
//     j = 0;
//     while (i < N)
//     {   
//         if (j < n)
//         {
//             in.push_back(1);
//             j++;
//             i++;
//         }
//         if (j < 2*n)
//         {
//             in.push_back(0);
//             j++;
//             i++;
//         }
//         j = 0;
//     }
//     cout << "\ti is: " << i << endl;

// }

void SignalGenerator::generateVariableDelayPwm(
        vector<int> &in,
        float duration, //total duration of the signal (s)
        float aRef,
        float delayShiftSec,  //how many secs this signal is shifted by
        float tSpan, //# of secs to reach to desired accel, aRef
        float initAngularVelocity,
        float samplingPeriod)
{ 
    DB_ENTER();
    float dt = samplingPeriod;
    uint N = INT(tSpan,dt);
    uint totalSamplesInSignal = duration/dt;
    DB_PRINT("N is %d", N);
    float aCur = 0; 
    float aPrev = aCur;
    float delaySec = (1.0f/initAngularVelocity);
    uint delay = INT(delaySec,dt);
    uint j = 0;
    uint n = 0;
    while ((in.size() < N ) && ( delay > samplingPeriod))
    {
        DB_PRINT("delay is %d", delay);
        j = 0;
        while (j < delay/2)
        {
            in.push_back(1);
            j++;
        }
        while (j <= delay)
        {
            in.push_back(0);
            j++;
        }
        n+=1;
        delay = computeDelayAccel(aRef, N, n);
    }
    DB_PRINT("in.size: %d", in.size());
    /* now, the pwm will be a constant frequency (same delay)
    until the signal duration (in samples) is reached
    */
    while (in.size() <= totalSamplesInSignal)
    {
        j = 0;
        while (j < delay/2)
        {
            in.push_back(1);
            j++;
        }
        while (j < delay)
        {
            in.push_back(0);
            j++;
        }
    }
    DB_PRINT("in.size: %d", in.size());
}
vector<float> SignalGenerator::generateSinusoid(
        float frequency, 
        float duration, 
        float phaseShiftDeg,  
        float samplingPeriod)
{
    float dt = samplingPeriod;
    float omega = 2 * M_PI * frequency; // Angular frequency (omega = 2 * pi * frequency)
    float phi = phaseShiftDeg*(M_PI/180);
    vector<float> sineWave;
    
    for (float t = 0; t < duration; t = t + dt) {
        sineWave.push_back(std::sin((omega * t))); // Calculate sin(w * t)
    }
    return sineWave;
}


void SignalGenerator::generatePulse(
        vector<float>& pulseTrain, // Store the pulse values (0 or 1)
        float periodSec, 
        float durationSec,  
        float samplingPeriod, 
        float phaseShiftSec) {
    float dt = samplingPeriod;
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


void SignalGenerator::generateUnitStepSignal(
                    std::vector<float>& unitStepSig, 
                    float duration, 
                    float startTime,  
                    float samplingPeriod)
    {
    float dt = samplingPeriod;

    //FIXME
}
