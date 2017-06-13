#include "NetworkLayer.h"
#include "ConfigManager.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

ConvolutionLayer::ConvolutionLayer(ConfigManager &cfg, std::string sectionName) {
	std::string filename_1;
	int isRand;

	cfg.getVal(sectionName, "numberOfNeurons", numberOfNeurons);
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
		filename_2 = sectionName + "_neuron" + std::to_string(i+1) + ".txt"; //WHY i+1 ???
		ConvNeuron neuron(cfg, sectionName, filename_2, isRand);
		neurons.push_back(neuron);
	}

}

void ConvolutionLayer::ProcessLayerInput(const TRIPLET &InputMaps, bool bp_on) {
		

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
		neurons[i].ProcessMaps(neuronPacket, bp_on);
	}

}

void ConvolutionLayer::ProcessBackProp(const TRIPLET &InputErrors) {
	for (size_t i = 0; i < numberOfNeurons; ++i) {
		TRIPLET neuronPacket;

		for (size_t j = 0; j < adjMatrix.size(); ++j) {
			// transposed adj matrix;
			if (adjMatrix[i][j]) {
				neuronPacket.push_back(InputErrors[j]);
			}
		}
		std::cout << "void ConvNeuron::processBProp" << std::endl;
		neurons[i].ProcessBProp(neuronPacket);
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
		for (size_t j = 0; j < width; ++j) {
			adjMatrixFile >> value;
			adjMatrix[i].push_back(value);
		}
	}
}

void ConvolutionLayer::GetOutput(TRIPLET &Output) {
	Output.resize(numberOfNeurons);
	for (int i = 0; i < numberOfNeurons; ++i) {
		neurons[i].GetOutput(Output[i]);
	}
}

void ConvolutionLayer::GetBPOutput(TRIPLET &Output) {
	Output.resize(numberOfNeurons);
	for (int i = 0; i < numberOfNeurons; ++i) {
		neurons[i].GetBPOutput(Output[i]);
	}
}

void ConvolutionLayer::ReadSingleCore(ConfigManager &cfg, std::string sectionName, int neuronNumber, MATRIX &resCore) {
	std::ifstream neuronCoreFile;
	int neuronCoreHeight, neuronCoreWidth;
	//read height and width from file;
	cfg.getVal(sectionName, "convMatrixHeight", neuronCoreHeight);
	cfg.getVal(sectionName, "convMatrixWidth", neuronCoreWidth);

	std::string fileName_1 = sectionName + "_neuron" + std::to_string(neuronNumber);
	std::string fileName_2;

	// Read filename if token present
	if(cfg.isTokenPresent(sectionName, fileName_1)) {
		cfg.getVal(sectionName, fileName_1, fileName_2);
	} else {
		// Else consider it to be in the current directory
		// and named as `sectionName'_neuron`i'.txt
		//
		// Thus let fileName_2 be fileName_1
		fileName_2 = fileName_1;
	}
	neuronCoreFile.open(fileName_2, std::ios::in);

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

void ConvolutionLayer::UpdateNeuronCore(const MATRIX& updMap, const int neuronNumber) {
	int neuronCoreHeight, neuronCoreWidth;
	int oldCoreHeight, oldCoreWidth;

	neuronCoreHeight = updMap.size();
	neuronCoreWidth = updMap[0].size();

	oldCoreHeight = neurons[neuronNumber].convMatrixHeight;
	oldCoreWidth = neurons[neuronNumber].convMatrixWidth;

	if ((oldCoreHeight != neuronCoreHeight) || (oldCoreWidth != neuronCoreWidth)) {
		std::string errorMsg("Incorrect convolutional core dimensions! Function ConvolutionLayer::updateNeuronCore(...)");
		throw std::invalid_argument(errorMsg);
	}
	
	neurons[neuronNumber].ConvCore.resize(neuronCoreHeight);
	for (size_t i = 0; i < neuronCoreHeight; ++i) {
		neurons[neuronNumber].ConvCore[i].resize(neuronCoreWidth);
		for (size_t j = 0; j < neuronCoreWidth; ++j) {
			neurons[neuronNumber].ConvCore[i][j] = updMap[i][j];
		}
	}
}

void ConvolutionLayer::UpdateAllCoresFromFiles(ConfigManager &cfg, std::string sectionName) {

	for (size_t i = 0; i < numberOfNeurons; ++i) {
		MATRIX readCore;

		ReadSingleCore(cfg, sectionName, i, readCore);

		UpdateNeuronCore(readCore, i);
	}
}

void ConvolutionLayer::WriteSingleCore(ConfigManager &cfg, std::string sectionName, int neuronNumber) {
	neurons[neuronNumber].WriteCoreToFile(sectionName + "_neuron" + std::to_string(neuronNumber));
}

void ConvolutionLayer::WriteCoresToFiles(ConfigManager &cfg, std::string sectionName) {
	for (size_t i = 0; i < neurons.size(); ++i) {
		neurons[i].WriteCoreToFile(sectionName + "_neuron" + std::to_string(i));
	}
}

ConvolutionLayer::~ConvolutionLayer() {
	std::cout << "conv dstr" << std::endl;
	
}
