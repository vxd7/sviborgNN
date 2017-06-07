#include <vector>
#include <string>
#include <cmath>

#include "SubsampleNeuron.h"
#include "ConfigManager.h"
#include "NetworkException.h"


SubsampleNeuron::SubsampleNeuron(ConfigManager& cfg, const std::string& sectionName) {
	cfg.getVal(sectionName, "subsampleCoeff", subsampleCoeff);
	cfg.getVal(sectionName, "neuronBias", neuronBias);
	cfg.getVal(sectionName, "exponentThresholdFunction", exponentThresholdFunction);
	
}

void SubsampleNeuron::subsampleMap(const std::vector<std::vector<double>>& inputMap, bool bp_on) {
	size_t inputMapHeight = inputMap.size();
	size_t inputMapWidth = inputMap[0].size();

	MATRIX inputMapLocalCopy = inputMap;

	// if map dimensions are not even then add zero vectors;
    if (inputMapWidth % 2 != 0) {
        inputMapWidth++;
        
        for (int i = 0; i < inputMapHeight; ++i) {
            inputMapLocalCopy[i].push_back(0.0);
        }
    }

    if (inputMapHeight % 2 != 0) {
        inputMapHeight++;
        std::vector<double> zero(inputMapWidth);
        inputMapLocalCopy.push_back(zero);
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
		InvalidResultArrayDimension InvalidResultArrayDimensionException;
		throw InvalidResultArrayDimensionException;
	}


	size_t fmapi = 0, fmapj = 0;

	for(size_t i = 0; i < inputMapHeight-1; i += 2, fmapi++) {
		for(size_t j = 0; j < inputMapWidth-1; j += 2, fmapj++) {
			double summ;
			summ = inputMapLocalCopy[i][j] + inputMapLocalCopy[i + 1][j] +
			   	inputMapLocalCopy[i][j + 1] + inputMapLocalCopy[i + 1][j + 1];

			summ *= subsampleCoeff;
			summ += neuronBias;

			if (bp_on) {
				bpDerivativeValue[fmapi][fmapj] = sigmoidTresholdFuncDerivative(summ);
			}

			summ = sigmoidTresholdFunc(summ);
			outputFeatureMap[fmapi][fmapj] = summ;
			
		}
        fmapj = 0;
	}
	
}

/* Treshold function
 * x - normalized signal from neuron
 * -1 <= x <= 1
 */
double SubsampleNeuron::sigmoidTresholdFunc(const double& x) {
	if((x < -1.0) || (x > 1.0)) {
		throw std::domain_error("Invalid argument range in sigmoidal neuron treshold function");
	}
	
    double res = 1.0 / (1.0 + exp((-1.0) * x*exponentThresholdFunction));
    return res;
}
double SubsampleNeuron::sigmoidTresholdFuncDerivative(const double& x) {
	// add exceptions;
	double res = exponentThresholdFunction*sigmoidTresholdFunc(x)*(1 - sigmoidTresholdFunc(x));
	return res;
}

std::vector<std::vector<double>>& SubsampleNeuron::getOuputMap() {
	return outputFeatureMap;
}
