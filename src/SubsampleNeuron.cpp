#include <vector>
#include <string>
#include <cmath>

#include "SubsampleNeuron.h"
#include "ConfigManager.h"
#include "NetworkException.h"


SubsampleNeuron::SubsampleNeuron(ConfigManager& cfg, std::string sectionName) {
	/* Get subsample coefficient from config file */
	cfg.getVal(sectionName, "subsampleCoeff", subsampleCoeff);
	cfg.getVal(sectionName, "neuronBias", neuronBias);
	cfg.getVal("general", "exponentThresholdFunction", exponentThresholdFunction);
	
}

void SubsampleNeuron::subsampleMap(std::vector<std::vector<double>>& inputMap) {
	size_t inputMapHeight = inputMap.size();
	size_t inputMapWidth = inputMap[0].size();

	// if map dimensions are not even then add zero vectors;
    if (inputMapWidth % 2 != 0) {
        inputMapWidth++;
        
        for (int i = 0; i < inputMapHeight; ++i) {
            inputMap[i].push_back(0.0);
        }
    }

    if (inputMapHeight % 2 != 0) {
        inputMapHeight++;
        std::vector<double> zero(inputMapWidth);
        inputMap.push_back(zero);
    }

	outputMapHeight = inputMapHeight/2;
	outputMapWidth = inputMapWidth/2;

    // set up the outputFeatureMap size;
    if ((outputMapWidth >= 1) && (outputMapHeight >= 1)) {
		outputFeatureMap.resize(outputMapHeight);
		for (int i = 0; i < outputFeatureMap.size(); i++) {
			outputFeatureMap[i].resize(outputMapWidth);
		}
    } else {
		throw InvalidResultArrayDimensionException;
	}


	size_t fmapi = 0, fmapj = 0;

	for(size_t i = 0; i < inputMapHeight-1; i += 2, fmapi++) {
		for(size_t j = 0; j < inputMapWidth-1; j += 2, fmapj++) {
			double summ;
			summ = inputMap[i][j] + inputMap[i + 1][j] + inputMap[i][j + 1] + inputMap[i + 1][j + 1];
			summ *= subsampleCoeff;
			summ += neuronBias;

			summ = tFunc(summ);
			outputFeatureMap[fmapi][fmapj] = summ;
		}
        fmapj = 0;
	}
	
}

double SubsampleNeuron::tFunc(double x) {
	x /= 255.0;
    double res = 1.0 / (1.0 + exp((-1.0) * x*exponentThresholdFunction));

    return res;
}

std::vector<std::vector<double>>& SubsampleNeuron::getOuputMap() {
	return outputFeatureMap;
}
