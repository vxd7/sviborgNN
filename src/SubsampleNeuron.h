#pragma once

#include <vector>
#include <string>
#include <utility>

#include "ConfigManager.h"
#include "CustomTypes.h"

class SubsampleNeuron {
private:
	MATRIX outputFeatureMap;
	MATRIX BPOutput;

	/* These are standart values and cannot be overriden
	 * See Yan LeKunn works for more info */
	static const int subsampleMapHeight = 2;
	static const int subsampleMapWidth = 2;


	double subsampleCoeff;
	double neuronBias;
	double exponentThresholdFunction;

	MATRIX bpDerivativeValue;

public:
	SubsampleNeuron(ConfigManager& cfg, const std::string& sectionName);

	void subsampleMap(const std::vector<std::vector<double>>& inputMap, bool bp_on = false);

	std::vector<std::vector<double>>& getOuputMap();
	double getMax(const std::vector<double>& elems);
    double sigmoidTresholdFunc(const double& x);
	double sigmoidTresholdFuncDerivative(const double& x);
	// processBProp expands the errors matrix!
	void processBProp(MATRIX &Errors);
	std::vector<std::vector<double>>& getBPOutput();

	int winning_unit_i;
	std::vector<std::pair<int,int>> winning_unit;
	int outputMapHeight;
	int outputMapWidth;

};
