#include "ConvLayer.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

ConvLayer::ConvLayer(ConfigManager &cfg, std::string sectionName) {
	std::string filename_1;
	bool isRand;

	cfg.getVal(sectionName, "numberofNeurons", numberOfNeurons);
	cfg.getVal(sectionName, "Random", isRand);
	if (numberOfNeurons <= 0) {
		std::string errorMsg("Incorrect neuron number! Function ConvLayer::ConvLayer(...)");

		throw std::invalid_argument(errorMsg);
	}

	cfg.getVal(sectionName, "AdjMatrix", filename_1);
	ReadAdjMatrix(filename_1);
	// read cores;
	for (int i = 0; i < numberOfNeurons; ++i) {
		std::string filename_2 = "";
		filename_2 = sectionName + "_neuron" + std::to_string(i);
		if (!isRand) {
			ConvNeuron::ConvNeuron(cfg, sectionName, filename_2);
		}
		else
			ConvNeuron::ConvNeuron(cfg, sectionName, filename_2, isRand);
	}

}

void ConvLayer::ComputeFeatureMap(MATRIX &InputMap) {
	for (int i = 0; i < numberOfNeurons; ++i) {
		neurons[i].Convolute(InputMap);
	}
}

void ConvLayer::ComputeFeatureMaps(TRIPLET &InputMaps) {


	/**
	* Construct packets of feature maps
	* according to the adjacency table adjMatrix
	*/

	for (size_t i = 0; i < numberOfNeurons; ++i) {
		std::vector<std::vector<std::vector<double>>> neuronPacket;

		for (size_t j = 0; j < adjMatrix.size(); ++j) {
			if (adjMatrix[j][i]) {
				neuronPacket.push_back(InputMaps[j + 1]);
			}
		}
		std::cout << "void ConvNeuron::processMaps(const std::vector<std::vector <std::vector<double>>> &inputMaps)" << std::endl;
		neurons[i].ProcessMaps(neuronPacket);
	}

}

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

void ConvLayer::readSingleCore(ConfigManager &cfg, std::string sectionName, int neuronNumber, MATRIX &resCore) {
	std::ifstream neuronCoreFile;
	int neuronCoreHeight, neuronCoreWidth;
	//read height and width from file;
	cfg.getVal(sectionName, "convMatrixHeight", neuronCoreHeight);
	cfg.getVal(sectionName, "convMatrixWidth", neuronCoreWidth);

	fileNamePrefix += "_neuron";
	std::string fileName = fileNamePrefix + std::to_string(neuronNumber);
	std::string fileName_1 = " ";
	cfg.getVal(sectionName, fileName, fileName_1);
	neuronCoreFile.open(fileName_1, std::ios::in);

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

void ConvLayer::updateNeuronCore(const MATRIX &updMap, const int neuronNumber) {
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