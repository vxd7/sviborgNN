#include "NetworkLayer.h"
#include "NetworkException.h"
#include <vector>
#include <string>

void SubsampleLayer::checkConfigValidity(const ConfigManager &cfg, const std::string& sectionName) {
	bool valid = true;

	std::vector<std::string> missingEntries;

	for(size_t i = 0; i < criticalConfigEntries.size(); ++i) {
		if(!cfg.isTokenPresent((sectionName), criticalConfigEntries[i])) {
			if(valid) {
				valid = false;
			}

			missingEntries.push_back(criticalConfigEntries[i]);
		}
	}

	if(!valid) {
		MissingConfiguration MissingConfigurationException(missingEntries);
		throw MissingConfigurationException;
	}

}

SubsampleLayer::SubsampleLayer(ConfigManager &cfg, std::string sectionName) {
	checkConfigValidity(cfg, sectionName);

	cfg.getVal(sectionName, "numberOfNeurons", numberOfNeurons);

	/* Resize neurons array */
	SubsampleNeuron tmpNeuron(cfg, sectionName);
	layerNeurons.resize(numberOfNeurons, tmpNeuron);


	if(cfg.isTokenPresent(sectionName, "outputMapDir")) {
		cfg.getVal(sectionName, "outputMapDir", outputMapDir);
	}
	
}
