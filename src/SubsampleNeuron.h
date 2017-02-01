#pragma once

#include <vector>
#include <string>

#include "ConfigManager.h"
class SubsampleNeuron {
private:
	std::vector<std::vector<double>> outputFeatureMap;

	/* These are standart values and cannot be overriden
	 * See Yan LeKunn works for more info */
	const int subsampleMapHeight = 2;
	const int subsampleMapWidth = 2;


	double subsampleCoeff;
	double neuronBias;
	double exponentThresholdFunction;

public:
	SubsampleNeuron(ConfigManager& cfg, std::string sectionName);

	void subsampleMap(std::vector<std::vector<double>>& inputMap);

	std::vector<std::vector<double>>& getOuputMap();
    double tFunc(double x);

	int outputMapHeight;
	int outputMapWidth;

};
