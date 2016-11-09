#pragma once

#include <iostream>
#include <vector>

// #include "ConvNeuron.h"
// #include "Log.h"

class ConvLayer {
private:
	int numberOfNeurons;

	int inputMapHeight;
	int inputMapWidth;

	std::vector <ConvNeuron> neurons;
public:
	ConvLayer();
	~ConvLayer();
	
	void initLayer(const int neuronCount, const int mapHeight, const int mapWidth);

	void computeFeatureMaps(std::vector<std::vector<double>> inputMap);

	void writeNeuronsToFiles(...);
	void readNeuronsFromFile(...);


};
