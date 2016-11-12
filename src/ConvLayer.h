#pragma once

#include <string>
#include <vector>

#include "ConvNeuron.h"
// #include "Log.h"

struct mapDim {
	int mapHeight;
	int mapWidth;
};

class ConvLayer {
private:
	int numberOfNeurons;

	mapDim inputMap;
	std::vector <mapDim> resultMaps;

	std::vector <ConvNeuron> neurons;

	void writeMapsToFiles(std::string fileNamePrefix);
	void readMapsFromFiles(std::string fileNamePrefix);

	void writeSingleMap(std::string fileNamePrefix, int neuronNumber);
	void readSingleMap(std::string fileNamePrefix, int neuronNumber);

	void writeCoresToFiles(std::string fileNamePrefix);
	void readCoresFromFiles(std::string fileNamePrefix);

	void updateNeuronCore(std::vector<std::vector<double>> &updMap, int neuronNumber);

public:
	ConvLayer(const int neuronCount);
	ConvLayer(const int neuronCount, const int inputImageHeight, const int inputImageWidth);

	~ConvLayer();

	void computeFeatureMaps(std::vector<std::vector<double>> inputMap);

};
