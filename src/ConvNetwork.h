#pragma once

#include <vector>
#include <utility>
#include <memory>

#include "NetworkLayer.h"
#include "ImageIface.h"
#include "CustomTypes.h"
#include "ConfigManager.h"

class ConvNetwork
{
private:
    int numLayers;
    double learningSpeed;
	std::vector <std::unique_ptr<NetworkLayer>> networkLayers;
	ConfigManager globalNetworkConfigurer;


	ImageIface inputImages;
    std::vector <double> desiredOutputMap;
public:
    // Initialization of Network;
    ConvNetwork(std::string configFileName);

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
	void BackPropLearning(std::vector<double>& outputMap, std::vector<double>& DesiredOutputMap, const double learningSpeed, const int inputMapNumber);

    double GradientNeuron();

	/******************
	 * Misc functions *
	 ******************/

};
