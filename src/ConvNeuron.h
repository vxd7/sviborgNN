#pragma once


class ConvNeuron {
private:
    std::vector<std::vector<double>> convCore;
    std::vector<std::vector<double>> outputFeatureMap; //Out of the neuron

    double aWeight;

    int inputMapHeight, inputMapLength;
    int coreHeight, coreLength;

    bool random;

    void randomize();

    double summate(const std::vector <std::vector<double>> &inputMap, int ipos, int jpos); // may be this is shit, but i wanted to optimise the code;
    double tFunc(double x);

public:
    ConvNeuron(int height, int length, bool isRand = false); // random weights if isRand = true, else pushback 0.0;
    ~ConvNeuron();

    void processMap(const std::vector <std::vector<double>> &inputMap, std::vector <std::vector<double>> &outputFeatureMap); // creating an output feautureMap;

    void initNeuron(); // empty for now;
};
