#pragma once
#include "MLP_Layer.h"
class Network
{
private:
	std::vector<Layer> Net;
public:
	Network(std::vector<int> layers);
	Network(std::string pathname, int numberlayers);
	void NetTrain(std::vector<double> Input, std::vector<double> Teacher, double SpeedTrain);
	void NetChain(std::vector<double> Input);
	void PrintOutput();
};