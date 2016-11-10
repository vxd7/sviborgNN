#pragma once

#include <string>
#include <vector>

#include "ConvNeuron.h"
// #include "Log.h"

class ConvLayer {
private:
	int numberOfNeurons;

	int inputMapHeight;
	int inputMapWidth;

	std::vector <ConvNeuron> neurons;

public:
	ConvLayer(const int neuronCount, const int mapHeight, const int mapWidth);
	~ConvLayer();

	void computeFeatureMaps(std::vector<std::vector<double>> inputMap);

	void writeMapsToFiles(std::string fileNamePrefix);
	void readMapsFromFiles(std::string fileNamePrefix);

	void writeSingleMap(std::string fileNamePrefix, int neuronNumber);
	void readSingleMap(std::string fileNamePrefix, int neuronNumber);

	void updateMap(std::vector<std::vector<double>> &updMap, int neuronNumber);
};
