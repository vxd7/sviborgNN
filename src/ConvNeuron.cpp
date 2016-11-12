#include <vector>
#include <cmath>    //exp()
#include <stdlib.h> //rand()

#include "ConvNeuron.h"

// we can write height, length in the file of weights in the begining and read them from it;
ConvNeuron::ConvNeuron(int height, int length, bool isRand /* = false*/){
    
    random = isRand;
    coreHeight = height;
    coreLength = length;

    convCore.resize(height);
    for (int i = 0; i < height; i++){
        convCore[i].assign(length,0.0);
    }

    if (random){
        randomizeCores();
    }
}
void ConvNeuron::initNeuron(){ 
    //here we can put randomization (instead of using it in constructor), and getting weights from the FILE with function getCore;
}
void ConvNeuron::processMap(const std::vector <std::vector<double>> &inputMap){
    double summ = 0;
    int fmapi = 0, fmapj = 0;
    // set up the featureMap size;
    outputFeatureMap.resize(inputMapHeight/coreHeight);
    for (int i = 0; i < outputFeatureMap.size(); i++){
        outputFeatureMap[i].resize(inputMapLength / coreLength);
    }
    // convolution;
    for (int i = 0; i < inputMapHeight; i = i + coreHeight){
        for (int j = 0; j < inputMapLength; j = j + coreLength){
            summ = summate(inputMap,i,j);
            outputFeatureMap[fmapi][fmapj] = tFunc(summ);
            fmapj++; 
        }
    }
}
double ConvNeuron::summate(const std::vector <std::vector<double>> &inputMap, int ipos, int jpos){
    double summ = 0;

    for (int i = 0; i < coreHeight; i++){
        for (int j = 0; j < coreLength; j++){
            summ = summ + inputMap[ipos][jpos] * convCore[i][j];
        }
    }

    return summ;
}
double ConvNeuron::tFunc(double x)
{
    double a;//= value?
    double Func = 1 / (1 + exp(-1 * x*a));

    return Func;
}
void ConvNeuron::randomizeCores(){

    for (int i = 0; i < coreHeight; i++){
        for (int j = 0; j < coreLength; j++){
            convCore[i][j] = (double)rand() / (double)(RAND_MAX)* 1.0 - 0.5;
        }
    }
    
}
void ConvNeuron::unloadFeatureMap() {
	outputFeatureMap.clear();
}
ConvNeuron::~ConvNeuron(){
    aWeight = 0;
    inputMapHeight = inputMapLength = 0;
    coreHeight = coreLength = 0;
}
