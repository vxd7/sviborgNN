#pragma once

#include <vector>

namespace MLP
{
	std::vector<std::vector<double>> MistakeWeights(const double &SpeedTrain, const std::vector<double> &Mistake, const std::vector<double> &BeforeOutput);
	std::vector<double> MistakeBias(const double &SpeedTrain, const std::vector<double> &Mistake);
	std::vector<double> SumMistake(const std::vector<std::vector<double>> &Weights, const std::vector<double> &Mistake);
}