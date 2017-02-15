#include <cmath>
#include <iostream>
#include "MLP_Layer.h"
#include "MLP_InitializationFunctions.h"
#include "MLP_VectorFunctions.h"
#include "MLP_Mistake.h"

using namespace MLP;

Layer::Layer(const int &NumberNeuronsBeforeLayer, const int &NumberNeuronsThisLayer, const std::string &weights, const std::string &bias)
{
	FileWeights = weights;
	Weights = InitializationWeights(NumberNeuronsBeforeLayer, NumberNeuronsThisLayer);
	WritingWeights(Weights, FileWeights);

	FileBias = bias;
	Bias = InitializationBias(NumberNeuronsThisLayer);
	WritingBias(Bias, FileBias);
}
Layer::Layer(const std::string &weights, const std::string &bias)
{
	FileWeights = weights;
	Weights = ReadingWeights(FileWeights);

	FileBias = bias;
	Bias = ReadingBias(FileBias);
}
Layer::Layer(const int &NumberThisLayer, const int &NumberNeuronsBeforeLayer, const int &NumberNeuronsThisLayer)
{
	FileWeights = "Layer" + std::to_string(NumberThisLayer) + "Weights.txt";
	Weights = InitializationWeights(NumberNeuronsBeforeLayer, NumberNeuronsThisLayer);
	WritingWeights(Weights, FileWeights);

	FileBias = "Layer" + std::to_string(NumberThisLayer) + "Bias.txt";
	Bias = InitializationBias(NumberNeuronsThisLayer);
	WritingBias(Bias, FileBias);
}
void Layer::InputData(const std::vector<double> &data)
{
	Input = data;
}
void Layer::Summer()
{
	Amounts.resize(Bias.size());
	double sum = 0;
	for (unsigned int i = 0; i < Bias.size(); i++)
	{
		sum = Bias[i];
		for (unsigned int j = 0; j < Input.size(); j++)
		{
			sum = sum + Input[j] * Weights[i][j];
			Amounts[i] = sum;
		}
		sum = 0;
	}
}
void Layer::TransferFunction()
{
	Output.resize(Amounts.size());
	for (unsigned int i = 0; i < Output.size(); i++)
	{
		Output[i] = tanh(Amounts[i]);
	}
}
void Layer::DerivativeTransferFunction()
{
	DerivativeOutput.resize(Amounts.size());
	for (unsigned int i = 0; i < DerivativeOutput.size(); i++)
	{
		DerivativeOutput[i] = 1.0 / pow(cosh(Amounts[i]), 2.0);
	}
}
void Layer::PrintOutput()
{
	for (unsigned int i = 0; i < Output.size(); i++)
	{
		std::cout << "Output[" << i + 1 << "] = " << Output[i] << "\t";
	}
	std::cout << std::endl;
}
std::vector<double> Layer::TransferOutput()
{
	return Output;
}
void Layer::LastLayerTrain(const std::vector<double> &Teacher, const double &SpeedTrain)
{
	MistakeOutput = (Teacher - Output)*DerivativeOutput;
	DeltaWeights = MistakeWeights(SpeedTrain, MistakeOutput, Input);
	DeltaBias = MistakeBias(SpeedTrain, MistakeOutput);
	Weights = Weights + DeltaWeights;
	Bias = Bias + DeltaBias;
	WritingWeights(Weights, FileWeights);
	WritingBias(Bias, FileBias);
	MistakeOutputOutput = SumMistake(Weights, MistakeOutput);
}
void Layer::HiddenLayerTrain(const std::vector<double> &MistakeBeforeLayer, const double &SpeedTrain)
{
	MistakeOutput = MistakeBeforeLayer*DerivativeOutput;
	DeltaWeights = MistakeWeights(SpeedTrain, MistakeOutput, Input);
	DeltaBias = MistakeBias(SpeedTrain, MistakeOutput);
	Weights = Weights + DeltaWeights;
	Bias = Bias + DeltaBias;
	WritingWeights(Weights, FileWeights);
	WritingBias(Bias, FileBias);
	MistakeOutputOutput = SumMistake(Weights, MistakeOutput);
}
std::vector<double> Layer::TransferMistakeOutputOutput()
{
	return MistakeOutputOutput;
}