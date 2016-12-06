#pragma once
#include <vector>

class ConvLayer;

class ConvNeuron {
private:
    std::vector<std::vector<double>> convCore;
    std::vector<std::vector<double>> outputFeatureMap; //Out of the neuron

    double aWeight;

    int coreHeight, coreWidth;

    bool random;
    void randomizeCores();

	double neuronBias = 1.0; // !!!
	double subsampleCoeff = 1.0;

    double summate(const std::vector <std::vector<double>> &inputMap, int ipos, int jpos); // may be this is shit, but i wanted to optimise the code;
    double tFunc(double x);

	/******************
	 * Misc functions *
	 ******************/

	/**
	 * Unloads the output feature map of the neuron
	 * For the better memory management
	 */
	void unloadFeatureMap();

public:
	// TODO: change hardcoded true
    ConvNeuron(int newCoreHeight, int newCoreWidth, bool isRand = true); // random weights if isRand = true, else pushback 0.0;

	ConvNeuron();

    void processMaps(const std::vector<std::vector <std::vector<double>>> &inputMaps); // creating an output feautureMap;

    void processSingleMap(const std::vector <std::vector<double>> &inputMap); // creating an output feautureMap;

	void subsampleMap(const std::vector<std::vector<double>> &inputMap);

    void initNeuron(); // empty for now;

	void changeBias(double newNeuronBias);
	void changeSubsCoeff(double newSubsCoeff);

	friend ConvLayer;
};
