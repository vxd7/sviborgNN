#pragma once
#include <vector>
#include <string>
class Layer
{
private:
	int NumberNeurons;
	std::vector<double> Input;
	std::vector<std::vector<double>> Weights;
	std::string FileWeights;
	std::vector<double> Bias;
	std::string FileBias;
	std::vector<double> Amounts;
	std::vector<double> Output;
	std::vector<double> DerivativeOutput;
	std::vector<double> MistakeOutput;
	std::vector<double> MistakeOutputOutput;
	std::vector<std::vector<double>> DeltaWeights;
	std::vector<double> DeltaBias;
public:
	Layer(int NumberNeuronsBeforeLayer, int NumberNeuronsThisLayer, std::string weights, std::string bias);
	Layer(std::string weights, std::string bias);
	Layer(int NumberThisLayer, int NumberNeuronsBeforeLayer, int NumberNeuronsThisLayer);
	void input(std::vector<double> data);
	void Summer();
	void TransferFunction();
	void DerivativeTransferFunction();
	void PrintOutput();
	std::vector<double> TransferOutput();
	void LastLayerTrain(std::vector<double> Teacher, double SpeedTrain);
	void HiddenLayerTrain(std::vector<double> MistakeBeforeLayer, double SpeedTrain);
	std::vector<double> Layer::TransferMistakeOutputOutput();
};