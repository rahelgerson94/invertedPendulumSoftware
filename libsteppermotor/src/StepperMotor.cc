#include "StepperMotor.hh"
#define IA 0
#define IB 1 
#define THETA  2
#define THETA_DOT 3
// Function to compute derivative of ia
// Function signature for RK4 integration
// Function signature as specified
// Extended integrate function to handle additional input parameters



float StepperMotor::computeIaDot(float Va) {
    
    float ia = _state[IA];
    float theta = _state[THETA];
    float omega = _state[THETA_DOT];
    
    return (Va - _Kt * omega * std::sin(_N * theta) - _R * ia) / _L;
}
void StepperMotor::updateIa(float Va) {
    
    float k1 = computeIaDot(Va);
    float k2 = computeIaDot(Va + (_dt/2)*k1);
    float k3 = computeIaDot(Va + (_dt/2)*k2);
    float k4 = computeIaDot(Va +   _dt*k3);
    float ia = _state[IA];
    _state[IA] = ia + (_dt / 6.0f) * (k1 + 2.0f * k2 + 2.0f * k3 + k4);
}

// Function to compute derivative of ib
float StepperMotor::computeIbDot( float Vb) {
    
    float ib = _state[IB];
    float theta = _state[THETA];
    float omega = _state[THETA_DOT];
    
    return (Vb + _Kt * omega * std::cos(_N * theta) - _R * ib) / _L;
}

void StepperMotor::updateIb(float Vb) {
    
    float ib = _state[IB] ; 
    float k1 = computeIbDot(Vb);
    float k2 = computeIbDot(Vb + (_dt/2)*k1);
    float k3 = computeIbDot(Vb + (_dt/2)*k2);
    float k4 = computeIbDot(Vb + _dt*k3);

    _state[1] = ib + (_dt / 6.0f) * (k1 + 2.0f * k2 + 2.0f * k3 + k4);
}

// Function to compute derivative of omega (thetaDot)
void StepperMotor::updateThetaDot( float loadTorque) {
    
    float thetaDot = _state[THETA_DOT] ; 
    float k1 = computeThetaDdot(loadTorque);
    float k2 = computeThetaDdot(loadTorque + (_dt/2)*k1);
    float k3 = computeThetaDdot(loadTorque + (_dt/2)*k2);
    float k4 = computeThetaDdot(loadTorque +  _dt*k3);

    _state[THETA_DOT] = thetaDot + (_dt / 6.0f) * (k1 + 2.0f * k2 + 2.0f * k3 + k4);
    
}

// Function to compute derivative of omega (thetaDot)
float StepperMotor::computeThetaDdot( float loadTorque) {
    vector<float> prevState;
    if (_stateHist.size() > 0)
    {
        prevState = _stateHist.back();
    }
    else
    {
        prevState = {0.0,0.0,0.0,0.0};
    }
    
   // PRINT_LINE
    float ia = prevState[IA];
    float ib = prevState[IB];
    float theta = prevState[THETA];
    float omega = prevState[THETA_DOT];
    
    return (-loadTorque + _Kt * ib * std::cos(_N * theta) - _Kt * ia * std::sin(_N * theta) - _Kv * omega -(Td*std::sin(_N*theta))) / _J;
}

void StepperMotor::updateStates(vector<float> inputVoltage, float loadTorque, float dt)
{
    _dt = dt;
    float Va = inputVoltage[0];
    float Vb = inputVoltage[1];

    // _state[0] =  integrate(computeIaDot,     ia,        Va,        dt);
    // _state[1] =  integrate(computeIbDot,     ib,        Vb,        dt);
    // _state[3] =  integrate(computeThetaDdot, thetadot,  loadTorque, dt);
    float theta = _state[2];
    updateIa(Va);
    updateIb(Vb);
    updateThetaDot(loadTorque);
    _state[2] =  theta + _state[3]*dt;
    _curTime += dt;
}

void StepperMotor::storeData(void)
{
    _stateHist.push_back(_state);
    _timeHist.push_back(_curTime);
}

void StepperMotor::save(std::string filename) 
{

        // Open the file
        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            throw std::runtime_error("Unable to open file for writing: " + filename);
        }
        // Save time and state history
        outFile << "t,ia,ib,θ,θdot\n";

        /*iterate over _stateHist, a vector of of size tf, where each row is a vector.
        each row has the form: t[i], ia[i], ib[i], θ[i],θdot[i] */
    
        for (uint t = 0; t < _stateHist.size(); ++t)
        {
            outFile << _timeHist[t];
            const vector<float> curVec = _stateHist[t];
            if (curVec.size() != 4) {throw std::runtime_error("numel in _stateHist[i] != 4! \n");}
            for (uint stateIt = 0; stateIt < (uint)curVec.size(); stateIt++)
            {
                outFile << "," << curVec[stateIt];
            }
            outFile << "\n";
        }
        outFile.close();
}

