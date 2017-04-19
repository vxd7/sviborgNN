#pragma once

#include <vector>
#include <utility>

#include "NetworkLayer.h"
#include "ImageIface.h"
#include "CustomTypes.h"

class ConvNetwork
{
private:
    int numLayers;
    int numLayeroutput;
    double learningSpeed;
    std::vector <double> desiredOutputMap;
	std::vector <NetworkLayer*> networkLayers;
	std::vector<int> NIL;//neurons in layers

	ImageIface inputImages;
public:
    // Initialization of Network;
    ConvNetwork(const std::vector<int> &neuronsInLayers, const std::vector<std::pair<int, int>> &convCoresDim);

    // Reading num of Layers from File;
    ConvNetwork();

    // Processing an output vector;
    void processInputMap(std::vector <double> &outputMap, int inputMapNumber = 0);

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
