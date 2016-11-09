#include "convneuron.h"

// we can write height, length in the file of weights in the begining and read them from it;
ConvNeuron::ConvNeuron(int height, int length, bool isRand = false){
    
    random = isRand;
    coreHeight = height;
    coreLength = length;

    convCore.resize(height);
    for (int i = 0; i < height; i++){
        convCore[i].assign(length,0.0);
    }

    if (random){
        randomWeights();
    }
}
void ConvNeuron::initNeuron(){ 
    //here we can put randomization (instead of using it in constructor), and getting weights from the FILE with function getCore;
}
void ConvNeuron::processMap(std::vector <std::vector<double>> &map){
    double summ = 0;
    int fmapi = 0, fmapj = 0;
    // set up the featureMap size;
    featureMap.resize(inputMapHeight/coreHeight);
    for (int i = 0; i < featureMap.size(); i++){
        featureMap[i].resize(inputMapLength / coreLength);
    }
    // convolution;
    for (int i = 0; i < inputMapHeight; i = i + coreHeight){
        for (int j = 0; j < inputMapLength; j = j + coreLength){
            summ = summate(map,i,j);
            featureMap[fmapi][fmapj] = tFunc(summ);
            fmapj++; 
        }
    }
    // rewriting input;
    map = featureMap;
}
double ConvNeuron::summate(const std::vector <std::vector<double>> &map, int ipos, int jpos){
    double summ = 0;

    for (int i = 0; i < coreHeight; i++){
        for (int j = 0; j < coreLength; j++){
            summ = summ + map[ipos][jpos] * convCore[i][j];
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
void ConvNeuron::randomWeights(){

    for (int i = 0; i < coreHeight; i++){
        for (int j = 0; j < coreLength; j++){
            convCore[i][j] = (double)rand() / (double)(RAND_MAX)* 1.0 - 0.5;
        }
    }
    
}
ConvNeuron::~ConvNeuron(){
    convCore.clear();
    featureMap.clear();
    aWeight = 0;
    inputMapHeight = inputMapLength = 0;
    coreHeight = coreLength = 0;
}