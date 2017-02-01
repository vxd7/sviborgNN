#pragma once

#include <vector>
#include <string>
class SubsampleNeuron {
private:
	std::vector<std::vector<double>> outputFeatureMap;

	int inputMapHeight;
	int inputMapWidth;

	double subsampleCoeff;

public:
	SubsampleNeuron(ConfigManager& cfg, std::string sectionName);

	void subsampleMap(std::vector<std::vector<double>>& inputMap);

	std::vector<std::vector<double>>& getOuputMap();

	int outputMapHeight;
	int outputMapWidth;

};
