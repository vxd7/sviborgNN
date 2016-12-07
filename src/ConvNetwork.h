#pragma once

#include <vector>
#include <utility>

#include "ConvLayer.h"
#include "ImageIface.h"

class ConvNetwork
{
private:
    int numLayers;
    double learningSpeed;
    std::vector <double> desiredOutputMap;
	std::vector <ConvLayer> networkLayers;

	ImageIface inputImages;
public:
    // Initialization of Network;
    ConvNetwork(const std::vector<int> &neuronsInLayers, const std::vector<std::pair<int, int>> &convCoresDim);

    // Reading num of Layers from File;
    ConvNetwork();

    // Processing an output vector;
    void processInputMap(int inputMapNumber, std::vector <double> &outputMap);
	//Process only first input map
    void processInputMap(std::vector <double> &outputMap);

    // Get list of images from FILE;
    void getInput(std::string imageListFile);

	/**********************
	 * Learning functions *
	 **********************/

    // Getting Learning Parameters for BP from FILE;
    void getBPLearningParameters();

    // BP learning;
    void BackPropLearning(std::vector <double> DesiredOutputMap, double learningSpeed);

    double GradientNeuron();

	/******************
	 * Misc functions *
	 ******************/

    // Delete Network;
    //~ConvNetwork();
};
