#pragma once
#include "ConvNeuron.h"
#include "SubsampleNeuron.h"
#include "ConfigManager.h"

#include <string>

class NetworkLayer {
private:

public:
	virtual void ProcessLayerInput(const TRIPLET& inputMapList) = 0;

	 // ************************************** // 
	 // * Feature map manipulation functions * //
	 // ************************************** // 

	/**
	 * Write a single feature map of the neuronNumber'th neuron
	 * File names are constructed like this:
	 * layer<NUM>_neuron<NUM>.fMap
	 */
	void WriteSingleMap(std::string fileNamePrefix, const int neuronNumber);

	/**
	 * Write all the feature maps -- output of the neurons to the files.
	 * File names are constructed like this:
	 * layer<NUM>_neuron<NUM>.fMap
	 */
	void WriteMapsToFiles(std::string fileNamePrefix);

	/**
	 * Read a single neuronNumber'th feature map from the file.
	 * Takes a 2-dim. vector as the destination
	 */
	void ReadSingleMap(std::string fileNamePrefix, const int neuronNumber, MATRIX& resMap);

	/**
	 * Unload all the feature maps from all the neurons
	 */
	void UnloadFeatureMaps();

	virtual void GetOutput(TRIPLET &Output) = 0;
};

class ConvolutionLayer : public NetworkLayer {
private:
	// All data here
	std::vector<ConvNeuron> neurons;
	int numberOfNeurons;
	MATRIX adjMatrix;

public:
	ConvolutionLayer(ConfigManager &cfg, std::string sectionName);

	/*Function ProcessLayerInput(const TRIPLET& inputMapList):
	* Creates an output of Convolutional layer. Each input map
	* is connected to certain neuron and it's connection is determined 
	* by adjacency matrix.
	*
	*/

	void ProcessLayerInput(const TRIPLET& inputMapList);
	
	// ********************************************** //
	// * Convolutional cores manipulation functions * //
	// ********************************************** //
	
	/**
	 * Write single core of the neuronNumber'th neuron to the file.
	 * File name is constructed like this:
	 * layer<NUM>_neuron<NUM>.fMap
	 */
	void WriteSingleCore(ConfigManager &cfg, std::string sectionName, int neuronNumber);

	/**
	 * Write all the convolutional core of all the neurons to the files.
	 */
	void WriteCoresToFiles(ConfigManager &cfg, std::string sectionName);

	/**
	 * Read single convolutional core of the neuronNumber'th neuron.
	 * Takes a 2-dim. vector as the destination
	 */
	void ReadSingleCore(ConfigManager &cfg, std::string sectionName, int neuronNumber, MATRIX &resCore);

	/**
	 * Update the convolutional core of the single neuron
	 */
	void UpdateNeuronCore(const MATRIX& updMap, const int neuronNumber);

	/**
	 * Update all the convolutional cores of all the neurons from the files described
	 * in certain section
	 */
	void UpdateAllCoresFromFiles(ConfigManager &cfg, std::string sectionName);
	
	void ReadAdjMatrix(std::string filename);

	void GetOutput(TRIPLET &Output);
};

class SubsampleLayer : public NetworkLayer {
private:
	int numberOfNeurons;
	std::vector<SubsampleNeuron> layerNeurons;

	std::vector<std::string> criticalConfigEntries = {
		"numberOfNeurons",
		"subsampleCoeff",
		"neuronBias",
		"exponentThresholdFunction"
	};

	std::string outputMapDir;

	void checkConfigValidity(ConfigManager &cfg, const std::string& sectionName);

public:
	SubsampleLayer(ConfigManager &cfg, const std::string& sectionName);

	void ProcessLayerInput(const TRIPLET& inputMapList);

	void GetOutput(TRIPLET &Output);

};
