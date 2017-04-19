#pragma once
#include <string>
#include <vector>
#include "ConfigManager.h"

#include "CustomTypes.h"

class ConvNeuron {
public:
	// Convolutional Neuron constructor. Type: public. 
	// newCoreHeight, newCoreWidth - conv.cores dimensions. 
	// isRand - flag for initialization random values.
	ConvNeuron(ConfigManager& cfg, std::string SectionName, std::string configFiledName, bool isRand = false);
	ConvNeuron(int dimHeigth, int dimWidth);

	void GetOutput(MATRIX &tmp);

	void ProcessMaps(const TRIPLET &inputMaps);

	void ResizeOutput(int InputMapHeight, int InputMapWidth);

	void WriteCoreToFile(std::string configFiledName);

	MATRIX ConvCore;

	int convMatrixHeight, convMatrixWidth;
private:
	MATRIX OutputMap;
	
	double bias = 1.0;

	void RandomizeCores();

	double summate(const MATRIX &InputMap, int ipos, int jpos);
	
	double tFunc(double x);
};
