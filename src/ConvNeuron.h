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

	void GetBPOutput(MATRIX &tmp);

	void ProcessMaps(const TRIPLET &inputMaps, bool bp_on = false);

	void ProcessBProp(const TRIPLET &deltas);

	void ResizeOutput(int InputMapHeight, int InputMapWidth);

	void ResizeBPOutput(int InputMapHeight, int InputMapWidth);

	void zero_padding(MATRIX& tmp, int height, int width);

	void WriteCoreToFile(std::string configFiledName);

	MATRIX ConvCore;

	int convMatrixHeight, convMatrixWidth;
private:
	MATRIX OutputMap;
	
	MATRIX BPOutput;

	MATRIX CoreUpdates;

	MATRIX bpDerivativeValue;

	double bias;

	void RandomizeCores();

	double summate(const MATRIX &InputMap, int ipos, int jpos);

	double transpose_summate(const MATRIX &InputMap, int ipos, int jpos);
	
	double tFunc(const double x);

	double tFuncDerivative(const double x);
};
