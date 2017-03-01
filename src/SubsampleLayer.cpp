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

SubsampleLayer::SubsampleLayer(ConfigManager &cfg, const std::string& sectionName) {
	checkConfigValidity(cfg, sectionName);

	cfg.getVal(sectionName, "numberOfNeurons", numberOfNeurons);

	/* Resize neurons array */
	layerNeurons.resize(numberOfNeurons, SubsampleNeuron(cfg, sectionName));


	if(cfg.isTokenPresent(sectionName, "outputMapDir")) {
		cfg.getVal(sectionName, "outputMapDir", outputMapDir);
	}
	
}

void SubsampleLayer::ProcessSingleInput(const MATRIX& inputMap, const int neuronIndex) {
	if(neuronIndex > numberOfNeurons) {
		throw std::out_of_range("Out of range exception in subsampling layer");
	}

	layerNeurons[neuronIndex].subsampleMap(inputMap);
	
}

void SubsampleLayer::ProcessMultipleInput(const TRIPLET& inputMapList) {
	
}
