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
