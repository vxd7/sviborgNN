#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <utility>

#include "ConvNetwork.h"
#include "NetworkException.h"
#include "ImageIface.h"

ConvNetwork::ConvNetwork(const std::vector<int> &neuronsInLayers, const std::vector<std::pair<int, int>> &convCoresDim) {

	int layersCount = neuronsInLayers.size();

	/**
	 * Convolutional Neural Network consists of
	 * convolutional and subsampling layer pairs.
	 * If numLayers is not even, then we cannot construct a correct 
	 * architecture. Throw an exception and warn user
	 *
	 * (layersCount + 1) because we count layers from 0 by default
	 */
	// if((layersCount + 1)%2 != 0) {
	// 	std::string errorMsg("Incorrect number of layers. numLayers is not even!");

	// 	throw std::invalid_argument(errorMsg);
	// }

	numLayers = layersCount;
	std::cout << numLayers;

	/* Construct the first layer -- CONV one*/
	std::cout << "ConvLayer::ConvLayer(const int neuronCount, const std::pair<int, int> coreDim, const int inputImageHeight, const int inputImageWidth)" <<std::endl;
	networkLayers.push_back(ConvLayer(neuronsInLayers[0], convCoresDim[0], 5, 5)); /* !HARDCODED INPUT W/H */
	NIL.push_back(neuronsInLayers[0]);

	/* Construct all the other layers */
	/* j is for the convCoresDim array -- convolution cores dimensions */
	for(size_t i = 1, j = 1; i < numLayers; ++i) {

		if(i%2 != 0) {
			std::cout << "ConvLayer::ConvLayer(const int neuronCount, const LayerType newLayerType) " << std::endl;
			networkLayers.push_back(ConvLayer(neuronsInLayers[i], SUBSAMPLE));
		} else {
			std::cout << "ConvLayer::ConvLayer(const int neuronCount, const std::pair<int, int> coreDim, const LayerType newLayerType) " << std::endl;
			networkLayers.push_back(ConvLayer(neuronsInLayers[i], convCoresDim[j], CONV));
			++j;
		}
        numLayeroutput = neuronsInLayers[i];
		NIL.push_back(neuronsInLayers[i]);
	}
}

ConvNetwork::ConvNetwork() {
	
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
	for (size_t i = 0; i < layerOutput.size(); ++i) {
		if ((layerOutput[i].size() != 1) || (layerOutput[i][0].size() != 1))
			validNetworkOutput = false;
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
