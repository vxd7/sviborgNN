#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <utility>

#include "ConvNetwork.h"
#include "NetworkException.h"
#include "ImageIface.h"

ConvNetwork::ConvNetwork(std::string configFileName) {
	if(configFileName.size() != 0) {
		globalNetworkConfigurer.readConfigFile(configFileName);
	}

	//Read the number of layers from config file
	globalNetworkConfigurer.getVal("global", "numLayers", numLayers);
	networkLayers.resize(numLayers);

	std::cout << "Constructing layers, allocating memory" << std::endl;
	for(int i = 0; i < numLayers; ++i) {
		if(i%2 != 0) {
			networkLayers[i] = new ConvolutionLayer(globalNetworkConfigurer, "conv" + std::to_string(i));
		} else {
			networkLayers[i] = new SubsampleLayer(globalNetworkConfigurer, "sbs" + std::to_string(i));
		}

	}
}


void ConvNetwork::processInputMap(std::vector <double> &outputMap, int inputMapNumber = 0) {
	/**
	 * Load an input map
	 */
	MATRIX inputMap;
	inputImages.computeGrayscaleMatrix(inputMapNumber, inputMap);
	// output for debugging
	std::cout << "Input Map" << std::endl;
	for (int i = 0; i < inputMap.size(); ++i) {
		for (int j = 0; j < inputMap[i].size(); ++j) {
			std::cout << inputMap[i][j] << " ";
		}
		std::cout << std::endl;
	}
	/*
	* And start computing! :D
	*/
	TRIPLET layerOutput;
	layerOutput.push_back(inputMap);

	for (size_t i = 0; i < numLayers; ++i) {

		networkLayers[i].ProcessLayerInput(layerOutput);
		networkLayers[i].GetOutput(layerOutput);


	}

	/* layerOutput now has the output of the last layer */
	/* Check result feature maps for validity */
	bool validNetworkOutput = true;
	for(size_t i = 0; i < layerOutput.size(); ++i) {
		if((layerOutput[i].size() != 1) || (layerOutput[i][0].size() != 1)) {
			validNetworkOutput = false;
			break;
		}
	}

	/* Construct the result of the network */
	if(validNetworkOutput) {
        for (size_t i = 0; i < layerOutput.size(); ++i) {
			outputMap.push_back(layerOutput[i][0][0]);
		}
	} else {
		throw /*InvalidResultArrayDimensionException*/;
	}
}

void ConvNetwork::processInputMap(std::vector <double> &outputMap) {
    //definetly need a loop! 
	std::cout << "ConvNetwork::processInputMap(int inputMapNumber, std::vector <double> &outputMap)" << std::endl;
	processInputMap(1, outputMap);
}

void ConvNetwork::getInput(std::string imageListFile) {
    // exception added;
    std::ifstream fin(imageListFile.c_str(), std::ios::in);
    try {
       fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    }
    catch (const std::ifstream::failure& e) {
        std::cerr << "Exception opening/reading file";
    }
	
	//if(!fin.good()) {
	//	//throw ex
	//}

	int number;
	fin >> number;

	std::vector<std::string> imageList;

	for(size_t i = 0; i < number; ++i) {
		std::string fileName;
		fin>>fileName;

		imageList.push_back(fileName);
	}
	fin.close();

	std::cout << "void getImageList(std::vector<std::string> &imageList)" << std::endl;
	inputImages.getImageList(imageList);
	std::cout << "void normalizeEverything()" << std::endl;
	inputImages.normalizeEverything();

}

ConvNetwork::~ConvNetwork() {
	for(int i = 0; i < networkLayers.size(); ++i) {
		delete networkLayers[i];
		
	}
}
