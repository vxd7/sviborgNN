#include "ConvNeuron.h"
#include "ConfigManager.h"
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <cmath>
#include <iostream>


ConvNeuron::ConvNeuron(ConfigManager &cfg, std::string sectionName, std::string configFiledName, bool isRand) {
	std::string filename;
	//read dimensions;
	cfg.getVal(sectionName, "convMatrixHeight", convMatrixHeight);
	cfg.getVal(sectionName, "convMatrixWidth", convMatrixWidth);
	ConvCore.resize(convMatrixHeight);
	if (!isRand) {
		cfg.getVal(sectionName, configFiledName, filename);
		std::ifstream VALS;
		VALS.open(filename, std::ios::in);
		for (int i = 0; i < convMatrixHeight; ++i) {
			double num;
			for (int j = 0; j < convMatrixWidth; ++j) {
				VALS >> num;
				ConvCore[i].push_back(num);
			}
		}
		VALS.close();
	}
	else {
		RandomizeCores();
		//writing random cores to file;
		WriteCoreToFile(configFiledName);
	}
}

ConvNeuron::ConvNeuron(int dimHeigth, int dimWidth) {
	convMatrixHeight = dimHeigth;
	convMatrixWidth = dimWidth;
	RandomizeCores();
}


void ConvNeuron::RandomizeCores() {
	std::mt19937 gen(std::random_device().operator()());
	std::uniform_real_distribution<> urd(-0.1, 1.175);
	ConvCore.resize(convMatrixHeight);
	for (int i = 0; i < convMatrixHeight; ++i) {
		for (int j = 0; j < convMatrixWidth; ++j) {
			double a = urd(gen); // following Le Cun instructions
			ConvCore[i].push_back(a);
		}
	}
}

void ConvNeuron::WriteCoreToFile(std::string configFiledName) {
	std::ofstream coreFile;
	std::string filename = configFiledName;
	//cfg.getVal(sectionName, sectionName, filename);
	coreFile.open(filename);
	for (int i = 0; i < convMatrixHeight; ++i) {
		for (int j = 0; j < convMatrixWidth; ++j) {
			coreFile << ConvCore[i][j] << ' ';
		}
		coreFile << std::endl;
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

void ConvNeuron::ProcessMaps(const TRIPLET &inputMaps) {

	double sectorSumm = 0;

	if (!inputMaps.size()) {
		std::string errorMsg("Incorrect neuron Input! Function ConvNeuron::ProcessMaps(...)");

		throw std::invalid_argument(errorMsg);

	}

	int inputMapHeight = inputMaps[0].size();
	int inputMapWidth = inputMaps[0][0].size();

	ResizeOutput(inputMapHeight, inputMapWidth);

	
		for (int j = 0; j < OutputMap.size(); ++j) {
			for (int k = 0; k < OutputMap[j].size(); ++k) {
				for (int i = 0; i < inputMaps.size(); ++i) {
					double summ = 0;
					summ = summate(inputMaps[i], i, j);
					summ = tFunc(summ);
					summ += bias;
					sectorSumm += summ;
				}
				OutputMap[j][k] += sectorSumm;
			}

		}
	


}


void ConvNeuron::ResizeOutput(int InputMapHeight, int InputMapWidth) {
	//resizing output feature map
	OutputMap.resize(InputMapHeight - convMatrixHeight + 1);
	for (int i = 0; i < OutputMap.size(); ++i) {
		OutputMap[i].resize(InputMapWidth - convMatrixWidth + 1);
	}

}
void ConvNeuron::GetOutput(MATRIX &tmp) {
	tmp = OutputMap;
}
