#include <cmath>
#include <iostream>
#include "MLP_Layer.h"
#include "MLP_InitializationFunctions.h"
#include "MLP_VectorFunctions.h"
#include "MLP_Mistake.h"
Layer::Layer(int NumberNeuronsBeforeLayer, int NumberNeuronsThisLayer, std::string weights, std::string bias)
{
	FileWeights = weights;
	Weights = InitializationWeights(NumberNeuronsBeforeLayer, NumberNeuronsThisLayer);
	WritingWeights(Weights, FileWeights);

	FileBias = bias;
	Bias = InitializationBias(NumberNeuronsThisLayer);
	WritingBias(Bias, FileBias);
}
Layer::Layer(std::string weights, std::string bias)
{
	FileWeights = weights;
	Weights = ReadingWeights(FileWeights);

	FileBias = bias;
	Bias = ReadingBias(FileBias);
}
Layer::Layer(int NumberThisLayer, int NumberNeuronsBeforeLayer, int NumberNeuronsThisLayer)
{
	FileWeights = "Layer" + std::to_string(NumberThisLayer) + "Weights.txt";
	Weights = InitializationWeights(NumberNeuronsBeforeLayer, NumberNeuronsThisLayer);
	WritingWeights(Weights, FileWeights);

	FileBias = "Layer" + std::to_string(NumberThisLayer) + "Bias.txt";
	Bias = InitializationBias(NumberNeuronsThisLayer);
	WritingBias(Bias, FileBias);
}
void Layer::input(std::vector<double> data)
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
void Layer::LastLayerTrain(std::vector<double> Teacher, double SpeedTrain)
{
	MistakeOutput = (Teacher - Output)*DerivativeOutput;

	//{
	//	std::cout << "MistakeOutput:Dimensions = " << MistakeOutput.size() << std::endl;
	//	std::cout << "MO[0] = " << MistakeOutput[0];
	//	for (unsigned int i = 1; i < MistakeOutput.size(); i++)
	//	{
	//		std::cout << "\tMO[" << i << "] = " << MistakeOutput[i];
	//	}
	//	std::cout << std::endl;
	//}

	DeltaWeights = MistakeWeights(SpeedTrain, MistakeOutput, Input);

	//{
	//	std::cout << "DeltaWeights:Dimensions = " << DeltaWeights.size() << " x " << DeltaWeights[0].size() << std::endl;
	//	std::cout << "DW[0,0] = " << DeltaWeights[0][0];
	//	for (unsigned int j = 1; j < DeltaWeights[0].size(); j++)
	//	{
	//		std::cout << "\tDW[0," << j << "] = " << DeltaWeights[0][j];
	//	}
	//	for (unsigned int i = 1; i < DeltaWeights.size(); i++)
	//	{
	//		std::cout << std::endl;
	//		std::cout << "DW[" << i << ",0] = " << DeltaWeights[i][0];
	//		for (unsigned int j = 1; j < DeltaWeights[0].size(); j++)
	//		{
	//			std::cout << "\tDW[" << i << "," << j << "] = " << DeltaWeights[i][j];
	//		}
	//	}
	//	std::wcout << std::endl;
	//}

	DeltaBias = MistakeBias(SpeedTrain, MistakeOutput);

	//{
	//	std::cout << "DeltaBias:Dimensions = " << DeltaBias.size() << std::endl;
	//	std::cout << "DB[0] = " << DeltaBias[0];
	//	for (unsigned int i = 1; i < DeltaBias.size(); i++)
	//	{
	//		std::cout << "\tDB[" << i << "] = " << DeltaBias[i];
	//	}
	//	std::cout << std::endl;
	//}

	Weights = Weights + DeltaWeights;

	//{
	//	std::cout << "Weights:Dimensions = " << Weights.size() << " x " << Weights[0].size() << std::endl;
	//	std::cout << "W[0,0] = " << Weights[0][0];
	//	for (unsigned int j = 1; j < Weights[0].size(); j++)
	//	{
	//		std::cout << "\tW[0," << j << "] = " << Weights[0][j];
	//	}
	//	for (unsigned int i = 1; i < Weights.size(); i++)
	//	{
	//		std::cout << std::endl;
	//		std::cout << "W[" << i << ",0] = " << Weights[i][0];
	//		for (unsigned int j = 1; j < Weights[0].size(); j++)
	//		{
	//			std::cout << "\tW[" << i << "," << j << "] = " << Weights[i][j];
	//		}
	//	}
	//	std::wcout << std::endl;
	//}

	Bias = Bias + DeltaBias;

	//{
	//	std::cout << "Bias:Dimensions = " << Bias.size() << std::endl;
	//	std::cout << "B[0] = " << Bias[0];
	//	for (unsigned int i = 1; i < Bias.size(); i++)
	//	{
	//		std::cout << "\tB[" << i << "] = " << Bias[i];
	//	}
	//	std::cout << std::endl;
	//}

	WritingWeights(Weights, FileWeights);
	WritingBias(Bias, FileBias);

	MistakeOutputOutput = SumMistake(Weights, MistakeOutput);
}
void Layer::HiddenLayerTrain(std::vector<double> MistakeBeforeLayer, double SpeedTrain)
{
	MistakeOutput = MistakeBeforeLayer*DerivativeOutput;

	//{
	//	std::cout << "MistakeOutput:Dimensions = " << MistakeOutput.size() << std::endl;
	//	std::cout << "MO[0] = " << MistakeOutput[0];
	//	for (unsigned int i = 1; i < MistakeOutput.size(); i++)
	//	{
	//		std::cout << "\tMO[" << i << "] = " << MistakeOutput[i];
	//	}
	//	std::cout << std::endl;
	//}

	DeltaWeights = MistakeWeights(SpeedTrain, MistakeOutput, Input);

	//{
	//	std::cout << "DeltaWeights:Dimensions = " << DeltaWeights.size() << " x " << DeltaWeights[0].size() << std::endl;
	//	std::cout << "DW[0,0] = " << DeltaWeights[0][0];
	//	for (unsigned int j = 1; j < DeltaWeights[0].size(); j++)
	//	{
	//		std::cout << "\tDW[0," << j << "] = " << DeltaWeights[0][j];
	//	}
	//	for (unsigned int i = 1; i < DeltaWeights.size(); i++)
	//	{
	//		std::cout << std::endl;
	//		std::cout << "DW[" << i << ",0] = " << DeltaWeights[i][0];
	//		for (unsigned int j = 1; j < DeltaWeights[0].size(); j++)
	//		{
	//			std::cout << "\tDW[" << i << "," << j << "] = " << DeltaWeights[i][j];
	//		}
	//	}
	//	std::wcout << std::endl;
	//}

	DeltaBias = MistakeBias(SpeedTrain, MistakeOutput);

	//{
	//	std::cout << "DeltaBias:Dimensions = " << DeltaBias.size() << std::endl;
	//	std::cout << "DB[0] = " << DeltaBias[0];
	//	for (unsigned int i = 1; i < DeltaBias.size(); i++)
	//	{
	//		std::cout << "\tDB[" << i << "] = " << DeltaBias[i];
	//	}
	//	std::cout << std::endl;
	//}

	Weights = Weights + DeltaWeights;

	//{
	//	std::cout << "Weights:Dimensions = " << Weights.size() << " x " << Weights[0].size() << std::endl;
	//	std::cout << "W[0,0] = " << Weights[0][0];
	//	for (unsigned int j = 1; j < Weights[0].size(); j++)
	//	{
	//		std::cout << "\tW[0," << j << "] = " << Weights[0][j];
	//	}
	//	for (unsigned int i = 1; i < Weights.size(); i++)
	//	{
	//		std::cout << std::endl;
	//		std::cout << "W[" << i << ",0] = " << Weights[i][0];
	//		for (unsigned int j = 1; j < Weights[0].size(); j++)
	//		{
	//			std::cout << "\tW[" << i << "," << j << "] = " << Weights[i][j];
	//		}
	//	}
	//	std::wcout << std::endl;
	//}

	Bias = Bias + DeltaBias;

	//{
	//	std::cout << "Bias:Dimensions = " << Bias.size() << std::endl;
	//	std::cout << "B[0] = " << Bias[0];
	//	for (unsigned int i = 1; i < Bias.size(); i++)
	//	{
	//		std::cout << "\tB[" << i << "] = " << Bias[i];
	//	}
	//	std::cout << std::endl;
	//}

	WritingWeights(Weights, FileWeights);
	WritingBias(Bias, FileBias);

	MistakeOutputOutput = SumMistake(Weights, MistakeOutput);
}
std::vector<double> Layer::TransferMistakeOutputOutput()
{
	return MistakeOutputOutput;
}