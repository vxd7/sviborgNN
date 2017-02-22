#pragma once
#include "ConvNeuron.h"
#include "SubsampleNeuron.h"
#include "ConfigManager.h"

#include <string>

class NetworkLayer {
private:

public:
	virtual void ProcessSingleInput(const MATRIX& inputMap) = 0;
	virtual void ProcessMultipleInput(const TRIPLET& inputMapList) = 0;

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

};

class ConvolutionalLayer : public NetworkLayer {
private:
	// All data here
	std::vector<ConvNeuron> layerNeurons;
	MATRIX adjMatrix;

public:
	ConvolutionalLayer(ConfigManager &cfg, std::string sectionName);

	void ProcessSingleInput(const MATRIX& inputMap);
	void ProcessMultipleInput(const TRIPLET& inputMapList);
	
	// ********************************************** //
	// * Convolutional cores manipulation functions * //
	// ********************************************** //
	
	/**
	 * Write single core of the neuronNumber'th neuron to the file.
	 * File name is constructed like this:
	 * layer<NUM>_neuron<NUM>.fMap
	 */
	void WriteSingleCore(std::string fileNamePrefix, int neuronNumber);

	/**
	 * Write all the convolutional core of all the neurons to the files.
	 * File names are constructed like this
	 * layer<NUM>_neuron<NUM>.cnvCore
	 */
	void WriteCoresToFiles(std::string fileNamePrefix);

	/**
	 * Read single convolutional core of the neuronNumber'th neuron.
	 * Takes a 2-dim. vector as the destination
	 */
	void ReadSingleCore(std::string fileNamePrefix, const int neuronNumber, MATRIX& resCore);

	/**
	 * Update the convolutional core of the single neuron
	 */
	void UpdateNeuronCore(const MATRIX& updMap, const int neuronNumber);

	/**
	 * Update all the convolutional cores of all the neurons from the files with
	 * fileNamePrefix prefix
	 */
	void UpdateAllCoresFromFiles(std::string fileNamePrefix);
	
	void ReadAdjMatrix(std::string filename);
};

class SubsampleLayer : public NetworkLayer {
private:
	std::vector<SubsampleNeuron> layerNeurons;

	std::vector<std::string> criticalConfigEntries = {
		"subsampleCoeff",
		"neuronBias",
		"exponentThresholdFunction"
	};

	std::string outputMapDir;

	void checkConfigValidity(const ConfigManager &cfg, const std::string& sectionName);
public:
	SubsampleLayer(ConfigManager &cfg, std::string sectionName);

	void ProcessSingleInput(const MATRIX& inputMap);
	void ProcessMultipleInput(const TRIPLET& inputMapList);

};
