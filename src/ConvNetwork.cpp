#include <stdexcept>
#include <utility>
#include "ConvNetwork.h"

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

void ConvNetwork::processOutput(std::vector <double> &outputMap) {
	
}