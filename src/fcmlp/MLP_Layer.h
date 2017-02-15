#pragma once
#include <vector>
#include <string>

namespace MLP
{
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
		Layer(const int &NumberNeuronsBeforeLayer, const int &NumberNeuronsThisLayer, const std::string &weights, const std::string &bias);
		Layer(const std::string &weights, const std::string &bias);
		Layer(const int &NumberThisLayer, const int &NumberNeuronsBeforeLayer, const int &NumberNeuronsThisLayer);
		void InputData(const std::vector<double> &data);
		void Summer();
		void TransferFunction();
		void DerivativeTransferFunction();
		void PrintOutput();
		std::vector<double> TransferOutput();
		void LastLayerTrain(const std::vector<double> &Teacher, const double &SpeedTrain);
		void HiddenLayerTrain(const std::vector<double> &MistakeBeforeLayer, const double &SpeedTrain);
		std::vector<double> Layer::TransferMistakeOutputOutput();
	};
}