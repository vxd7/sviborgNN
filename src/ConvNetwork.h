#pragma once
#include <vector>
#include <utility>

#include "ConvLayer.h"

class ConvNetwork
{
private:
    int numLayers;
    double learningSpeed;
    std::vector <double> DesiredOutputMap;
    std::vector <std::vector <double>> InputMap;
	std::vector <ConvLayer> networkLayers;
public:
    // Initialization of Network;
    ConvNetwork(const std::vector<int> neuronsInLayers, const std::vector<std::pair<int, int>> &convCoresDim);

    // Reading num of Layers from File;
    ConvNetwork();

    // Processing an output vector;
    void processOutput(std::vector <double> &outputMap);

    // Getting InputMap from FILE;
    void getInput();

    // Getting Learning Parameters for BP from FILE;
    void getBPLearningParameters();

    // BP learning;
    void BackPropLearning(std::vector <double> DesiredOutputMap, double learningSpeed);

    double GradientNeuron();

    // Delete Network;
    ~ConvNetwork();
};
