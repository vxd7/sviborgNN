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

// ************************************** // 
// * Feature map manipulation functions * //
// ************************************** // 

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
	std::string fileName = fileNamePrefix + std::to_string(neuronNumber) + ".fMap";
	
	//Empty file if something was there
	neuronMapFile.open(fileName, std::ios::out | std::ios::trunc);

	int neuronMapHeight, neuronMapWidth;
	neuronMapHeight = neurons[neuronNumber].outputFeatureMap.size();
	neuronMapWidth = neurons[neuronNumber].outputFeatureMap[0].size();

	neuronMapFile << neuronMapHeight << " " << neuronMapWidth << std::endl;
	for(size_t i = 0; i < neuronMapHeight; ++i) {
		for(size_t j = 0; j < neuronMapWidth; ++j) {
			neuronMapFile << neurons[neuronNumber].outputFeatureMap[i][j] << " ";
		}
		neuronMapFile << std::endl;
	}

	neuronMapFile.close();
}


void ConvLayer::readSingleMap(std::string fileNamePrefix, const int neuronNumber, std::vector <std::vector <double>> &resMap) {
	std::ifstream neuronMapFile;

	fileNamePrefix += "_neuron";
	std::string fileName = fileNamePrefix + std::to_string(neuronNumber) + ".fMap";

	neuronMapFile.open(fileName, std::ios::in);

	/**
	 * Read the height and width of the map
	 */
	int neuronMapHeight, neuronMapWidth;
	neuronMapFile >> neuronMapHeight >> neuronMapWidth;

	/**
	 * Check whether we have the space to read the map into
	 */
	if(resMap.size() != neuronMapHeight) {
		resMap.resize(neuronMapHeight);
	}

	for(size_t i = 0; i < neuronMapHeight; ++i) {
		if(resMap[i].size() != neuronMapWidth) {
			resMap[i].resize(neuronMapWidth);
		}
	}

	/**
	 * And read it!
	 */
	for(size_t i = 0; i < neuronMapHeight; ++i) {
		for(size_t j = 0; j < neuronMapWidth; ++j) {
			neuronMapFile >> resMap[i][j];
		}
	}

	neuronMapFile.close();
}

void ConvLayer::unloadFeatureMaps() {
	for(size_t i = 0; i < numberOfNeurons; ++i) {
		neurons[i].unloadFeatureMap();
	}
}

// ********************************************** //
// * Convolutional cores manipulation functions * //
// ********************************************** //

void ConvLayer::writeSingleCore(std::string fileNamePrefix, int neuronNumber) {
	std::ofstream neuronCoreFile;

	fileNamePrefix += "_neuron";
	std::string fileName = fileNamePrefix + std::to_string(neuronNumber) + ".fMap";

	neuronCoreFile.open(fileName, std::ios::out | std::ios::trunc);

	int neuronCoreHeight, neuronCoreWidth;

	neuronCoreHeight = neurons[neuronNumber].coreHeight;
	neuronCoreWidth = neurons[neuronNumber].coreLength;

	neuronCoreFile << neuronCoreHeight << " " << neuronCoreWidth;
	for(size_t i = 0; i < neuronCoreHeight; ++i) {
		for(size_t j = 0; j < neuronCoreWidth; ++j) {
			neuronCoreFile << neurons[neuronNumber].convCore[i][j] << " ";
		}
		neuronCoreFile << std::endl;
	}

	neuronCoreFile.close();
}

void ConvLayer::writeCoresToFiles(std::string fileNamePrefix) {
	for(size_t i = 0; i < numberOfNeurons; ++i) {
		writeSingleCore(fileNamePrefix, i);
	}
}

void ConvLayer::readSingleCore(std::string fileNamePrefix, const int neuronNumber, std::vector <std::vector <double>> &resCore) {
	std::ifstream neuronCoreFile;

	fileNamePrefix += "_neuron";
	std::string fileName = fileNamePrefix + std::to_string(neuronNumber) + ".fMap";

	neuronCoreFile.open(fileName, std::ios::in);

	int neuronCoreHeight, neuronCoreWidth;
	neuronCoreFile >> neuronCoreHeight >> neuronCoreWidth;

	if(resCore.size() != neuronCoreHeight) {
		resCore.resize(neuronCoreHeight);
	}

	for(size_t i = 0; i < neuronCoreHeight; ++i) {
		if(resCore[i].size() != neuronCoreWidth) {
			resCore[i].resize(neuronCoreWidth);
		}
	}

	for(size_t i = 0; i < neuronCoreHeight; ++i) {
		for(size_t j = 0; j < neuronCoreWidth; ++j) {
			neuronCoreFile >> resCore[i][j];
		}
	}

	neuronCoreFile.close();
}

void ConvLayer::updateNeuronCore(const std::vector<std::vector<double>> &updMap, const int neuronNumber) {
	int neuronCoreHeight, neuronCoreWidth;
	int oldCoreHeight, oldCoreWidth;

	neuronCoreHeight = updMap.size();
	neuronCoreWidth = updMap[0].size();

	oldCoreHeight = neurons[neuronNumber].coreHeight;
	oldCoreWidth = neurons[neuronNumber].coreLength;

	if( (oldCoreHeight != neuronCoreHeight) || (oldCoreWidth != neuronCoreWidth) ) {
		std::string errorMsg("Incorrect convolutional core dimensions! Function ConvLayer::updateNeuronCore(...)");
		throw std::invalid_argument(errorMsg);
	}

	for(size_t i = 0; i < neuronCoreHeight; ++i) {
		for(size_t j = 0; j < neuronCoreWidth; ++j) {
			neurons[neuronNumber].convCore[i][j] = updMap[i][j];
		}
	}
}

void ConvLayer::updateAllCoresFromFiles(std::string fileNamePrefix) {

	for(size_t i = 0; i < numberOfNeurons; ++i) {
		std::vector <std::vector <double>> readCore;

		readSingleCore(fileNamePrefix, i, readCore);

		updateNeuronCore(readCore, i);
	}
}
