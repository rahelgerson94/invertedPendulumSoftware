#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <cmath>
#include <fstream>
#include <string>
#include <stdexcept>

using std::function;
using std::vector;
using std::cout; 
using std::endl;
#define PRINT_LINE std::cout << "Line: " << __LINE__ << std::endl;

using uint = unsigned int;

const float g = 9.81; // m/s^2
class StepperMotor
{
    // Parameters for 17HS19-2004S
    // NOT from the 17HS19-2004S datasheet
    private:
        const float _T_hold = 0.59; // Nm
        const float _Kt = _T_hold / 2.0; // Nm/A
        const float _Kv = 0.01; 
        const float _L = 0.003; // mH
        const float _R = 1.4; // Ohm
        const float _Ts = 0.35; // Nm

        const float _Td = 15e-3; // Nm (detente torque)
        const float _J = 0.00162; //82e-7; // Kg m^2 (motor inertia)
        const float _mu = 0.55; // friction coefficient
        const float Td = 15 *10e-3; //Nm
        const float _Tmin = 0.075;
        const float _ddthetaMin = _Tmin / _J;
        const float _ddthetaMax = _Ts / _J; // m/s^2
        const uint _N = 100;
    public: 
        vector<vector<float>> _stateHist;
        vector<float> _timeHist;
        float _curTime = 0;
        float _dt;
        vector<float> _inputHist;
    public:
        vector<float> _state = {0,0,0,0}; // {ia, ib, θ, θdot}
        float computeIaDot(float Va);
        float computeIbDot(float Vb);
        float computeThetaDdot( float Tl);
        void run( vector<float> inputVoltage, float loadTorque, float dt, float duration);

        void save(std::string filename);
        void updateStates(vector<float> inputVoltage, float loadTorque, float dt);
        void storeData(void);
        void updateThetaDot(float);
        void updateIa(float);
        void updateIb(float);
        //float integrate(std::function<float(float)> stateDeriv, float state, float dt);
};

