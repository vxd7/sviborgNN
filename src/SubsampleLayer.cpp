#include "NetworkLayer.h"
#include "SubsampleNeuron.h"
#include "NetworkException.h"
#include <vector>
#include <string>
#include <exception>

void SubsampleLayer::checkConfigValidity(ConfigManager &cfg, const std::string& sectionName) {
	bool valid = true;

	std::vector<std::string> missingEntries;

	for(size_t i = 0; i < criticalConfigEntries.size(); ++i) {
		if(!cfg.isTokenPresent(sectionName, criticalConfigEntries[i])) {
			if(valid) {
				valid = false;
			}

			missingEntries.push_back(criticalConfigEntries[i]);
		}
	}

	if(!valid) {
		throw MissingConfiguration(missingEntries);
	}

}

//Constructor
SubsampleLayer::SubsampleLayer(ConfigManager &cfg, const std::string& sectionName) {
	checkConfigValidity(cfg, sectionName);

	cfg.getVal(sectionName, "numberOfNeurons", numberOfNeurons);

	/* Resize neurons array */
	layerNeurons.resize(numberOfNeurons, SubsampleNeuron(cfg, sectionName));


	if(cfg.isTokenPresent(sectionName, "outputMapDir")) {
		cfg.getVal(sectionName, "outputMapDir", outputMapDir);
	}
	
}

//Do subsampling
void SubsampleLayer::ProcessLayerInput(const TRIPLET& inputMapList, bool bp_on) {

	for(size_t i = 0; i < numberOfNeurons; ++i) {
		layerNeurons[i].subsampleMap(inputMapList[i], bp_on);
	}
}

void SubsampleLayer::ProcessBackProp(const TRIPLET &input) {
	for (size_t i = 0; i < numberOfNeurons; ++i) {
		layerNeurons[i].processBProp(input[i]);
	}
}

void SubsampleLayer::GetBPOutput(TRIPLET &Errors) {
	Errors.resize(numberOfNeurons);
	for (size_t i = 0; i < numberOfNeurons; ++i) {
		Errors[i] = layerNeurons[i].getBPOutput();
	}
}

void SubsampleLayer::GetOutput(TRIPLET &Output) {
	Output.resize(numberOfNeurons);
	for (size_t i = 0; i < numberOfNeurons; ++i) {
		Output[i] = layerNeurons[i].getOuputMap();
	}
}

SubsampleLayer::~SubsampleLayer() {
	std::cout << "sbs dstr" << std::endl;
	
}
