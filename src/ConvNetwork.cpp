#include <vector>
#include <string>
#include <stdexcept>
#include <utility>

#include "ConvNetwork.h"
#include "NetworkException.h"
#include "ImageIface.h"

ConvNetwork::ConvNetwork(const std::vector<int> neuronsInLayers, const std::vector<std::pair<int, int>> &convCoresDim) {

	int layersCount = neuronsInLayers.size();

	/**
	 * Convolutional Neural Network consists of
	 * convolutional and subsampling layer pairs.
	 * If numLayers is not even, then we cannot construct a correct 
	 * architecture. Throw an exception and warn user
	 *
	 * (layersCount + 1) because we count layers from 0 by default
	 */
	if((layersCount + 1)%2 != 0) {
		std::string errorMsg("Incorrect number of layers. numLayers is not even!");

		throw std::invalid_argument(errorMsg);
	}

	numLayers = layersCount;

	/* Construct the first layer -- CONV one*/
	networkLayers.push_back(ConvLayer(neuronsInLayers[0], convCoresDim[0], 5, 5)); /* !HARDCODED INPUT W/H */

	/* Construct all the other layers */
	/* j is for the convCoresDim array -- convolution cores dimensions */
	for(size_t i = 1, j = 1; i < numLayers; ++i) {

		if(i%2 != 0) {
			networkLayers.push_back(ConvLayer(neuronsInLayers[i], SUBSAMPLE));
		} else {
			networkLayers.push_back(ConvLayer(neuronsInLayers[i], convCoresDim[j], CONV));
			++j;
		}
	}
}

void ConvNetwork::processInputMap(int inputMapNumber, std::vector <double> &outputMap) {
	/* Image is loaded and network is built by now */


	/**
	 * Load an input map
	 */
	std::vector<std::vector<double>> inputMap;
	inputImages.computeGrayscaleMatrix(inputMapNumber, inputMap);

	/**
	 * And start the computation!
	 */
	std::vector<std::vector<std::vector<double>>> layerOutput;

	networkLayers[0].computeFeatureMap(inputMap);
	networkLayers[0].getAllFeatureMaps(layerOutput);

	for(size_t i = 1; i < numLayers; ++i) {
		if(networkLayers[i].type == CONV) {
			networkLayers[i].computeFeatureMaps(layerOutput);
		}

		if(networkLayers[i].type == SUBSAMPLE) {
			networkLayers[i].subsampleFeatureMaps(layerOutput);
		}

		networkLayers[i].getAllFeatureMaps(layerOutput);
	}

	//std::vector<std::vector<std::vector<double>>> lastLayerOutput;
	//int lastLayer = networkLayers.size() - 1;
	//networkLayers[lastLayer].getAllFeatureMaps(lastLayerOutput);

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
		for(size_t i = 0; i < layerOutput.size(); ++i) {
			outputMap.push_back(layerOutput[i][0][0]);
		}
	} else {
		throw InvalidResultArrayDimensionException;
	}
}

void ConvNetwork::processInputMap(std::vector <double> &outputMap) {
	processInputMap(0, outputMap);
}

void ConvNetwork::getInput(std::string imageListFile) {
	std::ifstream fin(imageListFile.c_str());

	if(!fin.good()) {
		//throw ex
	}

	int number;
	fin >> number;

	std::vector<std::string> imageList;

	for(size_t i = 0; i < number; ++i) {
		std::string fileName;
		fin>>fileName;

		imageList.push_back(fileName);
	}

	inputImages.getImageList(imageList);
	inputImages.normalizeEverything();
}
