#pragma once
#include <string>
#include <vector>
#include "ConfigManager.h"

typedef std::vector<std::vector<double>> MATRIX;
typedef std::vector<std::vector< std::vector<double>>> TRIPLET;

class ConvNeuron {
public:
	// Convolutional Neuron constructor. Type: public. 
	// newCoreHeight, newCoreWidth - conv.cores dimensions. 
	// isRand - flag for initialization random values.
	ConvNeuron(ConfigManager& cfg, std::string SectionName, std::string configFiledName, bool isRand = false);
	ConvNeuron(int dimHeigth, int dimWidth);

	void GetOutput(MATRIX &tmp);

	void Convolute(const MATRIX &InputMap);

	void ProcessMaps(const TRIPLET &inputMaps);

	void ResizeOutput(int InputMapHeight, int InputMapWidth);

	MATRIX ConvCore;

private:
	MATRIX OutputMap;
	int convMatrixHeight, convMatrixWidth;
	double bias = 1.0;

	void RandomizeCores();

	void WriteCoreToFile(ConfigManager &cfg, std::string sectionName, std::string configFiledName);
	
	double summate(const MATRIX &InputMap, int ipos, int jpos);
	
	double tFunc(double x);
};
