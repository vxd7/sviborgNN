#pragma once
#include "MLP_Layer.h"

namespace MLP
{
	class Network
	{
	private:
		std::vector<Layer> Net;
	public:
		Network(const std::vector<int> &layers);
		Network(const std::string &pathname, const int &numberlayers);
		void NetTrain(const std::vector<double> &Input, const std::vector<double> &Teacher, const double &SpeedTrain);
		void NetChain(const std::vector<double> &Input);
		void PrintOutput();
	};
}