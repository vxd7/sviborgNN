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

	 // ************************************** // 
	 // * Feature map manipulation functions * //
	 // ************************************** // 

	/**
	 * Write a single feature map of the neuronNumber'th neuron
	 * File names are constructed like this:
	 * layer<NUM>_neuron<NUM>.fMap
	 */
	void writeSingleMap(std::string fileNamePrefix, const int neuronNumber);

	/**
	 * Write all the feature maps -- output of the neurons to the files.
	 * File names are constructed like this:
	 * layer<NUM>_neuron<NUM>.fMap
	 */
	void writeMapsToFiles(std::string fileNamePrefix);

	/**
	 * Read a single neuronNumber'th feature map from the file.
	 * Takes a 2-dim. vector as the destination
	 */
	void readSingleMap(std::string fileNamePrefix, const int neuronNumber, std::vector <std::vector <double>> &resMap);

	/**
	 * Unload all the feature maps from all the neurons
	 */
	void unloadFeatureMaps();
	

	// ********************************************** //
	// * Convolutional cores manipulation functions * //
	// ********************************************** //
	
	/**
	 * Write single core of the neuronNumber'th neuron to the file.
	 * File name is constructed like this:
	 * layer<NUM>_neuron<NUM>.fMap
	 */
	void writeSingleCore(std::string fileNamePrefix, int neuronNumber);

	/**
	 * Write all the convolutional core of all the neurons to the files.
	 * File names are constructed like this
	 * layer<NUM>_neuron<NUM>.cnvCore
	 */
	void writeCoresToFiles(std::string fileNamePrefix);

	/**
	 * Read single convolutional core of the neuronNumber'th neuron.
	 * Takes a 2-dim. vector as the destination
	 */
	void readSingleCore(std::string fileNamePrefix, const int neuronNumber, std::vector <std::vector <double>> &resCore);

	/**
	 * Update the convolutional core of the single neuron
	 */
	void updateNeuronCore(const std::vector<std::vector<double>> &updMap, const int neuronNumber);

	/**
	 * Update all the convolutional cores of all the neurons from the files with
	 * fileNamePrefix prefix
	 */
	void updateAllCoresFromFiles(std::string fileNamePrefix);



public:
	ConvLayer(const int neuronCount);
	ConvLayer(const int neuronCount, const int inputImageHeight, const int inputImageWidth);

	~ConvLayer();

	/**
	 * Process the input feature maps of all the neurons
	 */
	void computeFeatureMaps(std::vector<std::vector<double>> inputMap);

};
