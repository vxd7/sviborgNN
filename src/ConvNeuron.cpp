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
	bias = 0.1;
}

ConvNeuron::ConvNeuron(int dimHeigth, int dimWidth) {
	convMatrixHeight = dimHeigth;
	convMatrixWidth = dimWidth;
	RandomizeCores();
	bias = 0.1;
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

double ConvNeuron::transpose_summate(const MATRIX &InputMap, int ipos, int jpos) {
	double summ = 0;
	for (int i = 0; i < convMatrixHeight; ++i) {
		for (int j = 0; j < convMatrixWidth; ++j) {
			summ += InputMap[i + ipos][j + jpos] * ConvCore[j][i];
		}
	}
	return summ;
}


double ConvNeuron::tFunc(const double x) {	
	/*double a =(10000*tanh(-1))/7615;*/
	return (10000 * tanh(x)) / 7615; // 10000/7615 coeff is amplitude which makes equalities f(1)=1 and f(-1)=-1 satisfied
}

double ConvNeuron::tFuncDerivative(const double x) {
	return 1 - tFunc(x)*tFunc(x);
}

void ConvNeuron::ProcessMaps(const TRIPLET &inputMaps, bool bp_on) {

	double sectorSumm = 0;
	double sectorSummBP = 0;

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
					// summ(w_i_j * a_i_j)
					summ = summate(inputMaps[i], j, k);
					if (bp_on) {
						double bp_summ = summ;
						bp_summ = tFuncDerivative(bp_summ);
						// in respect to the articles we should not add bias here...
						//bp_summ += bias;
						bpDerivativeValue[j][k] += bp_summ;
					}
					summ = tFunc(summ);
					summ += bias;
					sectorSumm += summ;
				}
				OutputMap[j][k] += sectorSumm;	
				sectorSumm = 0;

			}

		}
	


}


void ConvNeuron::ResizeOutput(int InputMapHeight, int InputMapWidth) {
	//resizing output feature map
	OutputMap.resize(InputMapHeight - convMatrixHeight + 1);
	for (size_t i = 0; i < OutputMap.size(); ++i) {
		OutputMap[i].resize(InputMapWidth - convMatrixWidth + 1);
	}

}
void ConvNeuron::ResizeBPOutput(int InputMapHeight, int InputMapWidth) {
	//resizing error map
	BPOutput.resize(OutputMap.size() + convMatrixHeight - 1);
	for (size_t i = 0; i < OutputMap.size(); ++i) {
		BPOutput[i].resize(OutputMap[i].size() + convMatrixWidth - 1);
	}
}

void ConvNeuron::GetOutput(MATRIX &tmp) {
	tmp = OutputMap;
}
// zero padding is required to reestablish input map size;
void ConvNeuron::zero_padding(MATRIX& tmp, int height, int width) {
	for (size_t i = 0; i < tmp.size(); ++i) {
		for (size_t j = 0; j < width - tmp[i].size(); ++j) {
			tmp[i].push_back(0.0);
		}
	}
	std::vector<double> zero(width);
	for (size_t i = 0; i < height - tmp.size(); ++i) {
		tmp.push_back(zero);
	}

}
// requires consideration!
void ConvNeuron::ProcessBProp(const TRIPLET &deltas) {
	int inputMapHeight, inputMapWidth;
	inputMapHeight = deltas[0].size();
	inputMapWidth = deltas[0][0].size();
	// BPOutput is for deltas
	// CoreUpdates is for d Errors/ d w

	ResizeBPOutput(inputMapHeight, inputMapWidth);
	// make a copy of TRIPLET to allow changes
	TRIPLET inputMapLocalCopy = deltas;

	for (size_t i = 0; i < deltas.size(); ++i) {
		// add zeroes and expand matrix. required for convcores product
		zero_padding(inputMapLocalCopy[i], BPOutput.size(), BPOutput[0].size());
	}
	// d Error / d output aka deltas
	for (int j = 0; j < inputMapHeight; ++j) {
		for (int k = 0; k < inputMapWidth; ++k) {
			double sectorSumm = 0;
			for (int i = 0; i < deltas.size(); ++i) {
				double summ = 0;
				summ = transpose_summate(inputMapLocalCopy[i], j, k);
				sectorSumm += summ;
			}
			BPOutput[j][k] += sectorSumm;
		}

	}
	// d Error/ d weight
	for (size_t i = 0; i < BPOutput.size(); ++i) {
		for (size_t j = 0; j < BPOutput.size(); ++j) {
			CoreUpdates[i].push_back(BPOutput[i][j] * OutputMap[i][j]);
		}
	}
	
}

void ConvNeuron::GetBPOutput(MATRIX &tmp) {
	tmp = BPOutput;
}