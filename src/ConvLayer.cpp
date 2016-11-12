#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

#include "ConvLayer.h"

ConvLayer::ConvLayer(const int neuronCount) {
	if(neuronCount <= 0) {
		std::string errorMsg("Incorrect neuron number! Function ConvLayer::ConvLayer(...)");

		throw std::invalid_argument(errorMsg);
	}

	numberOfNeurons = neuronCount;
	neurons.resize(numberOfNeurons);
}

ConvLayer::ConvLayer(const int neuronCount, const int inputImageHeight, const int inputImageWidth) {
	if(neuronCount <= 0) {
		std::string errorMsg("Incorrect neuron number! Function ConvLayer::ConvLayer(...)");

		throw std::invalid_argument(errorMsg);
	}

	numberOfNeurons = neuronCount;
	neurons.resize(numberOfNeurons);

	inputMap.mapHeight = inputImageHeight;
	inputMap.mapWidth = inputImageWidth;
}

//TODO: include neuron fixes
void ConvLayer::computeFeatureMaps(std::vector<std::vector<double>> inputMap) {
	for(size_t i = 0; i < numberOfNeurons; ++i) {
		neurons[i].processMap(inputMap);
	}
}

void ConvLayer::writeMapsToFiles(std::string fileNamePrefix) {
	for(size_t i = 0; i < numberOfNeurons; ++i) {
		writeSingleMap(fileNamePrefix, i);
	}
}

void ConvLayer::writeSingleMap(std::string fileNamePrefix, const int neuronNumber) {
	std::ofstream neuronMapFile;

	fileNamePrefix += "_neuron";
	std::string fileName= fileNamePrefix + std::to_string(neuronNumber) + ".txt";
	
	//Empty file if something was there
	neuronMapFile.open(fileName, std::ios::out | std::ios::trunc);

	int neuronMapHeight, neuronMapWidth;
	neuronMapHeight = neurons[neuronNumber].outputFeatureMap.size();
	neuronMapWidth = neurons[neuronNumber].outputFeatureMap[0].size();

	neuronMapFile << neuronMapHeight << " " << neuronMapWidth << std::endl;
	for(size_t i = 0; i < neuronMapHeight; ++i) {
		for(size_t j = 0; j < neuronMapWidth; ++j) {
			neuronMapFile << neurons[neuronNumber].outputFeatureMap[i][j];
		}
	}

	neuronMapFile.close();

}
