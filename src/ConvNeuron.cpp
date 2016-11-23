#include <vector>
#include <cmath>    //exp()
#include <stdlib.h> //rand()

#include "ConvNeuron.h"

// we can write height, length in the file of weights in the begining and read them from it;
ConvNeuron::ConvNeuron(int height, int length, bool isRand /* = false*/) {
    
    random = isRand;
    coreHeight = height;
    coreWidth = length;

    convCore.resize(height);
    for (int i = 0; i < height; i++) {
        convCore[i].assign(length,0.0);
    }

    if (random) {
        randomizeCores();
    }
}
void ConvNeuron::initNeuron() { 
    //here we can put randomization (instead of using it in constructor), and getting weights from the FILE with function getCore;
}
void ConvNeuron::processMaps(const std::vector<std::vector <std::vector<double>>> &inputMaps) {

	std::vector <double> mapSums;

    int fmapi = 0, fmapj = 0;

	int inputMapsCount = inputMaps.size();

	int inputMapHeight = inputMaps[0].size();
	int inputMapWidth = inputMaps[0][0].size();

    // set up the featureMap size;
    outputFeatureMap.resize(inputMapHeight/coreHeight);
    for (int i = 0; i < outputFeatureMap.size(); i++) {
        outputFeatureMap[i].resize(inputMapWidth/ coreWidth);
    }

    // convolution;
	
	for (int i = 0; (i + coreHeight) < inputMapHeight; ++i) {
		for (int j = 0; (j + coreWidth) < inputMapWidth; ++j) {

			for(int k = 0; k < inputMapsCount; ++k) {
				double summ;

				summ = summate(inputMaps[k], i, j);
				summ = tFunc(summ);
				mapSums.push_back(summ);
			}

			double pixelSum = 0.0;
			for(int m = 0; m < mapSums.size(); ++m) {
				pixelSum += mapSums[m];
			}

			outputFeatureMap[fmapi][fmapj] = pixelSum;
			mapSums.clear();

			fmapj++; 
		}

		fmapi++;
	}

}

void ConvNeuron::subsampleMap(const std::vector<std::vector<double>> &inputMap) {
	size_t inputMapHeight = inputMap.size();
	size_t inputMapWidth = inputMap[0].size();

	size_t fmapi = 0, fmapj = 0;

	for(size_t i = 0; i < inputMapHeight; i += 2, fmapi++) {
		for(size_t j = 0; j < inputMapWidth; j += 2, fmapj++) {
			double summ;
			summ = inputMap[i][j] + inputMap[i + 1][j] + inputMap[i][j + 1] + inputMap[i + 1][j + 1];
			summ *= subsampleCoeff;
			summ += neuronBias;

			summ = tFunc(summ);
			outputFeatureMap[fmapi][fmapj] = summ;
		}
	}
}

double ConvNeuron::summate(const std::vector <std::vector<double>> &inputMap, int ipos, int jpos) {
    double summ = 0;

    for (int i = 0; i < coreHeight; i++, ipos++){
        for (int j = 0; j < coreWidth; j++, jpos++){
            summ = summ + inputMap[ipos][jpos] * convCore[i][j];

        }
    }

    return summ;
}

double ConvNeuron::tFunc(double x) {
    double a = 1.0;//= value?
    double Func = 1 / (1 + exp(-1 * x*a));

    return Func;
}

void ConvNeuron::randomizeCores() {

    for (int i = 0; i < coreHeight; i++) {
        for (int j = 0; j < coreWidth; j++) {
            convCore[i][j] = (double)rand() / (double)(RAND_MAX)* 1.0 - 0.5;
        }
    }
    
}

void ConvNeuron::unloadFeatureMap() {
	for(size_t i = 0; i < outputFeatureMap.size(); ++i) {
		outputFeatureMap.clear();
	}
}
