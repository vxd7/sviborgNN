#include <vector>
#include <cmath>    //exp()
#include <stdlib.h> //rand()
#include <ctime>
#include "ConvNeuron.h"

// we can write height, length in the file of weights in the begining and read them from it;
ConvNeuron::ConvNeuron(int newCoreHeight, int newCoreWidth, bool isRand /* = false*/) {
    
    random = isRand;
    coreHeight = newCoreHeight;
    coreWidth = newCoreWidth;

	neuronBias = 1.0;
	subsampleCoeff = 1.0;

    convCore.resize(coreHeight);
    for (int i = 0; i < coreHeight; i++) {
        convCore[i].assign(coreWidth,0.0);
    }

    if (random) {
        randomizeCores();
    }
}
ConvNeuron::ConvNeuron() {
	
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
    outputFeatureMap.resize(inputMapHeight - (coreHeight - 1));
    for (int i = 0; i < outputFeatureMap.size(); i++) {
        outputFeatureMap[i].resize(inputMapWidth - (coreWidth - 1));
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
        fmapj = 0;
		fmapi++;
	}

}

void ConvNeuron::processSingleMap(const std::vector <std::vector<double>> &inputMap) {
	int fmapi = 0, fmapj = 0;

	int inputMapHeight = inputMap.size();
	int inputMapWidth = inputMap[0].size();

    // set up the featureMap size;
    outputFeatureMap.resize(inputMapHeight - (coreHeight - 1));
    for (int i = 0; i < outputFeatureMap.size(); i++) {
        outputFeatureMap[i].resize(inputMapWidth - (coreWidth - 1));
    }

	for (int i = 0; (i + coreHeight) < inputMapHeight; ++i) {
		for (int j = 0; (j + coreWidth) < inputMapWidth; ++j) {
			double summ;

			summ = summate(inputMap, i, j);
			summ = tFunc(summ);

			outputFeatureMap[fmapi][fmapj] = summ;

			fmapj++; 
		}
        fmapj = 0;
		fmapi++;
	}
}

void ConvNeuron::subsampleMap( std::vector<std::vector<double>> &inputMap) {
	size_t inputMapHeight = inputMap.size();
	size_t inputMapWidth = inputMap[0].size();

	size_t fmapi = 0, fmapj = 0;

	size_t subMapHeight = 2, subMapWidth = 2;

    // set up the outputFeatureMap size;
    outputFeatureMap.resize(inputMapHeight - (subMapHeight- 1));
    for (int i = 0; i < outputFeatureMap.size(); i++) {
        outputFeatureMap[i].resize(inputMapWidth - (subMapWidth- 1));
    }
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

double ConvNeuron::summate(const std::vector <std::vector<double>> &inputMap, int ipos, int jpos) {
    double summ = 0;
    int k=ipos, m=jpos;
    for (int i = 0; i < coreHeight; i++, k++){
        for (int j = 0; j < coreWidth; j++, m++){
            summ = summ + inputMap[ipos][jpos] * convCore[i][j];

        }
        m = jpos;

    }

    return summ;
}

double ConvNeuron::tFunc(double x) {
    double a = 1.0;//= value?
    double Func = 1 / (1 + exp(-1 * x*a));

    return Func;
}

void ConvNeuron::randomizeCores() {
    srand(time(NULL));
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

void ConvNeuron::changeBias(double newNeuronBias) {
	neuronBias = newNeuronBias;
}

void ConvNeuron::changeSubsCoeff(double newSubsCoeff) {
	subsampleCoeff = newSubsCoeff;
}
