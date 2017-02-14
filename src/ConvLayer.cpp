#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>

#include "ConvLayer.h"

/**
* General constructor
*/
ConvLayer::ConvLayer(const int neuronCount, const LayerType newLayerType) {
	type = newLayerType;

	if (neuronCount <= 0) {
		std::string errorMsg("Incorrect neuron number! Function ConvLayer::ConvLayer(...)");

		throw std::invalid_argument(errorMsg);
	}

	numberOfNeurons = neuronCount;
	neurons.resize(numberOfNeurons);
}

/**
* Constructor which takes convolutional core dimensions
*/
ConvLayer::ConvLayer(const int neuronCount, const std::pair<int, int> coreDim, const LayerType newLayerType) {
	type = newLayerType;

	if (neuronCount <= 0) {
		std::string errorMsg("Incorrect neuron number! Function ConvLayer::ConvLayer(...)");

		throw std::invalid_argument(errorMsg);
	}

	numberOfNeurons = neuronCount;
	neurons.resize(numberOfNeurons, ConvNeuron(coreDim.first, coreDim.second, true)); /* !HARDCODED TRUE */

}

/* Constructor for the first layer */
ConvLayer::ConvLayer(const int neuronCount, const std::pair<int, int> coreDim, const int inputImageHeight, const int inputImageWidth) {

	/* First layer is considered convolutional by default */
	type = CONV;

	if (neuronCount <= 0) {
		std::string errorMsg("Incorrect neuron number! Function ConvLayer::ConvLayer(...)");

		throw std::invalid_argument(errorMsg);
	}

	numberOfNeurons = neuronCount;
	neurons.resize(numberOfNeurons, ConvNeuron(coreDim.first, coreDim.second, true)); /* ! HARDCODED TRUE ! */

	inputMap.mapHeight = inputImageHeight;
	inputMap.mapWidth = inputImageWidth;
}

// ************************************** // 
// * Feature map manipulation functions * //
// ************************************** // 

void ConvLayer::ReadAdjMatrix(std::string filename) {
	std::ifstream adjMatrixFile;
	adjMatrixFile.open(filename, std::ios::in);
	bool value;
	int length, width;
	//std::vector<std::vector<bool>> adjMatrix;
	if (!adjMatrixFile.is_open()) {
		// throw exception
	}
	adjMatrixFile >> length >> width;
	adjMatrix.resize(length);
	for (size_t i = 0; i < length; ++i) {
		adjMatrix[i].resize(width);
		for (size_t j = 0; j < width; ++j) {
			adjMatrixFile >> value;
			adjMatrix[i].push_back(value);
		}
	}
}

//TODO: include neuron fixes
void ConvLayer::computeFeatureMaps(std::vector<std::vector<std::vector<double>>> &inputMap) {


	/**
	* Construct packets of feature maps
	* according to the adjacency table adjMatrix
	*/
	std::cout << "creating an AdjMatrix" << std::endl;
	static const int arr1[] = { 1, 0, 1, 1 };
	std::vector<bool> vec1(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]));
	static const int arr2[] = { 1, 1, 0, 1 };
	std::vector<bool> vec2(arr2, arr2 + sizeof(arr2) / sizeof(arr2[0]));
	static const int arr3[] = { 0, 1, 1, 0 };
	std::vector<bool> vec3(arr3, arr3 + sizeof(arr3) / sizeof(arr3[0]));


	adjMatrix.push_back(vec1);
	adjMatrix.push_back(vec2);
	adjMatrix.push_back(vec3);

	//std::string filename;//adjMatrix filename;
	//ReadAdjMatrix(filename);
	std::cout << "creating a neuron packets" << std::endl;
	for (size_t i = 0; i < numberOfNeurons; ++i) {
		std::vector<std::vector<std::vector<double>>> neuronPacket;

		for (size_t j = 0; j < adjMatrix.size(); ++j) {
			if (adjMatrix[j][i]) {
				neuronPacket.push_back(inputMap[inputMap.size() - numberOfNeurons + j + 1]);
			}
		}
		std::cout << "void ConvNeuron::processMaps(const std::vector<std::vector <std::vector<double>>> &inputMaps)" << std::endl;
		neurons[i].processMaps(neuronPacket);
	}

}

void ConvLayer::computeFeatureMap(std::vector<std::vector<double>> &inputMap) {
	int j;
	for (size_t i = inputMap.size() - numberOfNeurons, j = 0; i < inputMap.size(); ++i, ++j) {
		std::cout << "void ConvNeuron::processSingleMap(const std::vector <std::vector<double>> &inputMap)" << std::endl;
		neurons[j].processSingleMap(inputMap);
	}

}

void ConvLayer::subsampleFeatureMaps(std::vector<std::vector<std::vector<double>>> &inputMap) {
	int j;
	for (size_t i = inputMap.size() - numberOfNeurons, j = 0; i < inputMap.size(); ++i, ++j) {
		std::cout << "void ConvNeuron::subsampleMap(std::vector<std::vector<double>> &inputMap)" << std::endl;
		neurons[j].subsampleMap(inputMap[i]);
	}
}

void ConvLayer::writeMapsToFiles(std::string fileNamePrefix) {
	for (size_t i = 0; i < numberOfNeurons; ++i) {
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
	for (size_t i = 0; i < neuronMapHeight; ++i) {
		for (size_t j = 0; j < neuronMapWidth; ++j) {
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
	if (resMap.size() != neuronMapHeight) {
		resMap.resize(neuronMapHeight);
	}

	for (size_t i = 0; i < neuronMapHeight; ++i) {
		if (resMap[i].size() != neuronMapWidth) {
			resMap[i].resize(neuronMapWidth);
		}
	}

	/**
	* And read it!
	*/
	for (size_t i = 0; i < neuronMapHeight; ++i) {
		for (size_t j = 0; j < neuronMapWidth; ++j) {
			neuronMapFile >> resMap[i][j];
		}
	}

	neuronMapFile.close();
}

void ConvLayer::unloadFeatureMaps() {
	for (size_t i = 0; i < numberOfNeurons; ++i) {
		neurons[i].unloadFeatureMap();
	}
}

void ConvLayer::getAllFeatureMaps(std::vector<std::vector<std::vector<double>>> &resultMaps) {
	for (size_t i = 0; i < numberOfNeurons; ++i) {
		resultMaps.push_back(neurons[i].outputFeatureMap);
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
	neuronCoreWidth = neurons[neuronNumber].coreWidth;

	neuronCoreFile << neuronCoreHeight << " " << neuronCoreWidth;
	for (size_t i = 0; i < neuronCoreHeight; ++i) {
		for (size_t j = 0; j < neuronCoreWidth; ++j) {
			neuronCoreFile << neurons[neuronNumber].convCore[i][j] << " ";
		}
		neuronCoreFile << std::endl;
	}

	neuronCoreFile.close();
}

void ConvLayer::writeCoresToFiles(std::string fileNamePrefix) {
	for (size_t i = 0; i < numberOfNeurons; ++i) {
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

	if (resCore.size() != neuronCoreHeight) {
		resCore.resize(neuronCoreHeight);
	}

	for (size_t i = 0; i < neuronCoreHeight; ++i) {
		if (resCore[i].size() != neuronCoreWidth) {
			resCore[i].resize(neuronCoreWidth);
		}
	}

	for (size_t i = 0; i < neuronCoreHeight; ++i) {
		for (size_t j = 0; j < neuronCoreWidth; ++j) {
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
	oldCoreWidth = neurons[neuronNumber].coreWidth;

	if ((oldCoreHeight != neuronCoreHeight) || (oldCoreWidth != neuronCoreWidth)) {
		std::string errorMsg("Incorrect convolutional core dimensions! Function ConvLayer::updateNeuronCore(...)");
		throw std::invalid_argument(errorMsg);
	}

	for (size_t i = 0; i < neuronCoreHeight; ++i) {
		for (size_t j = 0; j < neuronCoreWidth; ++j) {
			neurons[neuronNumber].convCore[i][j] = updMap[i][j];
		}
	}
}

void ConvLayer::updateAllCoresFromFiles(std::string fileNamePrefix) {

	for (size_t i = 0; i < numberOfNeurons; ++i) {
		std::vector <std::vector <double>> readCore;

		readSingleCore(fileNamePrefix, i, readCore);

		updateNeuronCore(readCore, i);
	}
}