#include <stdexcept>
#include <utility>
#include <string>

#include "ConvNetwork.h"
#include "NetworkException.h"
#include "ImageBMP.h"

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
	networkLayers.push_back(ConvLayer(neuronsInLayers[0], convCoresDim[0], 0, 0)); /* !HARDCODED INPUT W/H */

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

void ConvNetwork::processInput(std::vector <double> &outputMap) {
	/* Image is loaded and network is built by now */

	networkLayers[0].computeFeatureMap(inputMap);

	for(size_t i = 1; i < numLayers; ++i) {
		std::vector<std::vector<std::vector<double>>> layerOutput;

		networkLayers[i].getAllFeatureMaps(layerOutput);

		if(networkLayers[i].type == CONV) {
			networkLayers[i].computeFeatureMaps(layerOutput);
		}

		if(networkLayers[i].type == SUBSAMPLE) {
			networkLayers[i].subsampleFeatureMaps(layerOutput);
		}
	}

	std::vector<std::vector<std::vector<double>>> lastLayerOutput;
	int lastLayer = networkLayers.size() - 1;

	networkLayers[lastLayer].getAllFeatureMaps(lastLayerOutput);

	/* Check result feature maps for validity */
	bool validNetworkOutput = true;
	for(size_t i = 0; i < lastLayerOutput.size(); ++i) {
		if(lastLayerOutput[i].size() != 1 && lastLayerOutput[i][0].size() != 1) {
			validNetworkOutput = false;
			break;
		}
	}

	/* Construct the result of the network */
	if(validNetworkOutput) {
		for(size_t i = 0; i < lastLayerOutput.size(); ++i) {
			outputMap.push_back(lastLayerOutput[i][0][0]);
		}
	} else {
		throw InvalidResultArrayDimensionException;
	}
}
