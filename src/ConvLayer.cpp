#include "NetworkLayer.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

ConvolutionLayer::ConvolutionLayer(ConfigManager &cfg, std::string sectionName) {
	std::string filename_1;
	bool isRand;

	cfg.getVal(sectionName, "numberofNeurons", numberOfNeurons);
	cfg.getVal(sectionName, "Random", isRand);
	if (numberOfNeurons <= 0) {
		std::string errorMsg("Incorrect neuron number! Function ConvolutionLayer::ConvolutionLayer(...)");

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

void ConvolutionLayer::ProcessSingleInput(const MATRIX &InputMap) {
	for (int i = 0; i < numberOfNeurons; ++i) {
		neurons[i].Convolute(InputMap);
	}
}

void ConvolutionLayer::ProcessMultipleInput(const TRIPLET &InputMaps) {


	/**
	* Construct packets of feature maps
	* according to the adjacency table adjMatrix
	*/

	for (size_t i = 0; i < numberOfNeurons; ++i) {
		TRIPLET neuronPacket;

		for (size_t j = 0; j < adjMatrix.size(); ++j) {
			if (adjMatrix[j][i]) {
				//check indexes in case of changing the numeration;
				neuronPacket.push_back(InputMaps[j]);
			}
		}
		std::cout << "void ConvNeuron::processMaps(const std::vector<std::vector <std::vector<double>>> &inputMaps)" << std::endl;
		neurons[i].ProcessMaps(neuronPacket);
	}

}

void ConvolutionLayer::ReadAdjMatrix(std::string filename) {
	std::ifstream adjMatrixFile;
	adjMatrixFile.open(filename, std::ios::in);
	bool value;
	int length, width;

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

// I cannot understandd how to work with configmanager;

void ConvolutionLayer::ReadSingleCore(ConfigManager &cfg, std::string sectionName, int neuronNumber, MATRIX &resCore) {
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

void ConvolutionLayer::updateNeuronCore(const MATRIX &updMap, const int neuronNumber) {
	int neuronCoreHeight, neuronCoreWidth;
	int oldCoreHeight, oldCoreWidth;

	neuronCoreHeight = updMap.size();
	neuronCoreWidth = updMap[0].size();

	oldCoreHeight = neurons[neuronNumber].coreHeight;
	oldCoreWidth = neurons[neuronNumber].coreWidth;

	if ((oldCoreHeight != neuronCoreHeight) || (oldCoreWidth != neuronCoreWidth)) {
		std::string errorMsg("Incorrect convolutional core dimensions! Function ConvolutionLayer::updateNeuronCore(...)");
		throw std::invalid_argument(errorMsg);
	}

	for (size_t i = 0; i < neuronCoreHeight; ++i) {
		for (size_t j = 0; j < neuronCoreWidth; ++j) {
			neurons[neuronNumber].convCore[i][j] = updMap[i][j];
		}
	}
}

void ConvolutionLayer::updateAllCoresFromFiles(std::string fileNamePrefix) {

	for (size_t i = 0; i < numberOfNeurons; ++i) {
		MATRIX readCore;

		readSingleCore(fileNamePrefix, i, readCore);

		updateNeuronCore(readCore, i);
	}
}

void ConvolutionLayer::WriteSingleCore(std::string fileNamePrefix, int neuronNumber) {

}

void ConvolutionLayer::WriteCoresToFiles(std::string fileNamePrefix) {
	for (size_t i = 0; i< )
}