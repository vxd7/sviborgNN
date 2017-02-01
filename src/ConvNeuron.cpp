#include "ConvNeuron.h"
#include "ConfigManager.h"
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <math.h>
#include <iostream>


ConvNeuron::ConvNeuron(ConfigManager &cfg, std::string sectionName, std::string configFiledName, bool isRand) {
	std::string filename;
	//read dimensions;
	cfg.getVal(sectionName, "convMatrixHeight", convMatrixHeight);
	cfg.getVal(sectionName, "convMatrixWidth", convMatrixWidth);
	//get filename with values;
	cfg.getVal(sectionName,configFiledName,filename);
	std::ifstream VALS;
	VALS.open(filename, std::ios::in);
	ConvCore.resize(convMatrixHeight);
	if (isRand) {
		for (int i = 0; i < convMatrixHeight; ++i) {
			double num;
			for (int j = 0; j < convMatrixWidth; ++j) {
				VALS >> num;
				ConvCore[i].push_back(num);
			}
		}
	}
	else
		RandomizeCores();
	VALS.close();
}

ConvNeuron::ConvNeuron(int dimHeigth, int dimWidth) {
	convMatrixHeight = dimHeigth;
	convMatrixWidth = dimWidth;
	RandomizeCores();
}


void ConvNeuron::RandomizeCores() {
	srand(time(NULL));
	ConvCore.resize(convMatrixHeight);
	for (int i = 0; i < convMatrixHeight; ++i) {
		for (int j = 0; j < convMatrixWidth; ++j) {
			double a = ((double)rand()/ (double)(RAND_MAX)*4.8 - 2.4)/(convMatrixHeight*convMatrixWidth); // following Le Cun instructions
			ConvCore[i].push_back(a);
		}
	}
}

void ConvNeuron::WriteCoreToFile(ConfigManager &cfg, std::string sectionName, std::string configFiledName) {
	std::ofstream FILE;
	std::string filename;
	cfg.getVal(sectionName, sectionName, filename);
	FILE.open(filename);
	for (int i = 0; i < convMatrixHeight; ++i) {
		for (int j = 0; j < convMatrixWidth; ++j) {
			FILE << ConvCore[i][j] << ' ';
		}
		FILE << std::endl;
	}
}

void ConvNeuron::Convolute(const MATRIX &InputMap) {

	//resizing output feature map
	OutputMap.resize(InputMap.size() - convMatrixHeight + 1);
	for (int i = 0; i < OutputMap.size(); ++i) {
		OutputMap[i].resize(InputMap[i].size() - convMatrixWidth + 1);
	}

	// convolution
	for (int i = 0; i < OutputMap.size(); ++i) {
		for (int j = 0; j < OutputMap[i].size(); ++j) {
			double summ;
			summ = summate(InputMap,i,j);
			summ = tFunc(summ);
			summ += bias;
			OutputMap[i][j] = summ;
		}
	}


}

double ConvNeuron::summate(const MATRIX &InputMap, int ipos, int jpos) {
	double summ = 0;
	for (int i = 0; i < convMatrixHeight; ++i) {
		for (int j = 0; j < convMatrixWidth; ++j) {
			summ += InputMap[i+ipos][j+jpos]*ConvCore[i][j];
		}
	}
	return summ;
}

double ConvNeuron::tFunc(double x) {	
	/*double a =(10000*tanh(-1))/7615;*/
	return (10000 * tanh(x)) / 7615; // 10000/7615 coeff is amplitude which makes equalities f(1)=1 and f(-1)=-1 satisfied
}

void ConvNeuron::GetOutput(MATRIX &tmp) {
	tmp = OutputMap;
}