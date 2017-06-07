#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <utility>
#include <memory>

#include "ConvNetwork.h"
#include "NetworkException.h"
#include "ImageIface.h"

ConvNetwork::ConvNetwork(std::string configFileName) {
	if(configFileName.size() != 0) {
		globalNetworkConfigurer.readConfigFile(configFileName);
	}

	//Read the number of layers from config file
	globalNetworkConfigurer.getVal("global", "numLayers", numLayers);

	std::cout << "Constructing layers" << std::endl;
	try {
		for(int i = 0; i < numLayers; ++i) {
			if(i%2 == 0) {
				std::cout << "Constructing " << "conv" << i << std::endl;
				std::unique_ptr<NetworkLayer> nlPtr(new ConvolutionLayer(globalNetworkConfigurer, 
							"conv" + std::to_string(i)));

				networkLayers.push_back(std::move(nlPtr));
			} else {
				std::cout << "Constructing " << "sbs" << i << std::endl;
				std::unique_ptr<NetworkLayer> nlPtr(new SubsampleLayer(globalNetworkConfigurer, 
							"sbs" + std::to_string(i)));

				networkLayers.push_back(std::move(nlPtr));
			}

		}
	} catch(std::exception& ex) {
		std::cout << ex.what() << std::endl;
		throw;
	}
}


void ConvNetwork::processInputMap(std::vector <double> &outputMap, int inputMapNumber) {
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

		networkLayers[i] -> ProcessLayerInput(layerOutput);
		networkLayers[i] -> GetOutput(layerOutput);

	}

	/* layerOutput now has the output of the last layer */
	/* Check result feature maps for validity */
	bool validNetworkOutput = true;
	for (size_t i = 0; i < layerOutput.size(); ++i) {
		if ((layerOutput[i].size() != 1) || (layerOutput[i][0].size() != 1)) {
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
