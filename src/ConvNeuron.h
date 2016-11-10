#pragma once
#include <vector>
#include <math.h>

class ConvNeuron {
private:
    std::vector<std::vector<double>> convCore;
    std::vector<std::vector<double>> featureMap;

    double aWeight;

    int inputMapHeight, inputMapLength;
    int coreHeight, coreLength;

    bool random;

    void randomWeights();

    double summate(const std::vector <std::vector<double>> &map, int ipos, int jpos); // may be this is shit, but i wanted to optimise the code;
    double tFunc(double x);

public:
    ConvNeuron(int height, int length, bool isRand = false); // random weights if isRand = true, else pushback 0.0;
    ~ConvNeuron();

    void processMap(std::vector <std::vector<double>> &map); // creating an output feautureMap;

    void initNeuron(); // empty for now;
};
