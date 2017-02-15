#include "MLP_Network.h"
#include <iostream>

using namespace MLP;

Network::Network(const std::vector<int> &layers)
{
	for (unsigned int i = 0; i < layers.size() - 1; i++)
	{
		Net.push_back(Layer(i + 1, layers[i], layers[i + 1]));
	}
}
Network::Network(const std::string &pathname, const int &numberlayers)
{
	for (int i = 1; i <= numberlayers; i++)
	{
		Net.push_back(Layer(pathname + "\\Layer" + std::to_string(i) + "Weights.txt", pathname + "\\layer" + std::to_string(i) + "Bias.txt"));
	}
}
void Network::NetChain(const std::vector<double> &Input)
{
	Net[0].InputData(Input);
	Net[0].Summer();
	Net[0].TransferFunction();
	for (unsigned int i = 1; i < Net.size(); i++)
	{
		Net[i].InputData(Net[i - 1].TransferOutput());
		Net[i].Summer();
		Net[i].TransferFunction();
	}
}
void Network::PrintOutput()
{
	Net[Net.size() - 1].PrintOutput();
}
void Network::NetTrain(const std::vector<double> &Input, const std::vector<double> &Teacher, const double &SpeedTrain)
{
	Net[0].InputData(Input);
	Net[0].Summer();
	Net[0].TransferFunction();
	Net[0].DerivativeTransferFunction();
	for (unsigned int i = 1; i < Net.size(); i++)
	{
		Net[i].InputData(Net[i - 1].TransferOutput());
		Net[i].Summer();
		Net[i].TransferFunction();
		Net[i].DerivativeTransferFunction();
	}
	Net[Net.size() - 1].LastLayerTrain(Teacher, SpeedTrain);
	for (unsigned int i = Net.size() - 1; i > 0; i--)
	{
		Net[i - 1].HiddenLayerTrain(Net[i].TransferMistakeOutputOutput(), SpeedTrain);
	}
}