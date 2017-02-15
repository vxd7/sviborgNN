#include "MLP_Mistake.h"
#include <iostream>

std::vector<std::vector<double>> MLP::MistakeWeights(const double &SpeedTrain, const std::vector<double> &Mistake, const std::vector<double> &BeforeOutput)
{
	std::vector<std::vector<double>> data(Mistake.size());
	for (unsigned int i = 0; i < data.size(); i++)
	{
		data[i].resize(BeforeOutput.size());
	}
	for (unsigned int i = 0; i < data.size(); i++)
	{
		for (unsigned int j = 0; j < data[0].size(); j++)
		{
			data[i][j] = SpeedTrain*Mistake[i] * BeforeOutput[j];
		}
	}
	return data;
}
std::vector<double> MLP::MistakeBias(const double &SpeedTrain, const std::vector<double> &Mistake)
{
	std::vector<double> data(Mistake.size());
	for (unsigned int i = 0; i < data.size(); i++)
	{
		data[i] = SpeedTrain*Mistake[i];
	}
	return data;
}
std::vector<double> MLP::SumMistake(const std::vector<std::vector<double>> &Weights, const std::vector<double> &Mistake)
{
	std::vector<double> data(Weights[0].size());
	for (unsigned int i = 0; i < data.size(); i++)
	{
		double sum = 0;
		for (unsigned int j = 0; j < Mistake.size(); j++)
		{
			sum = sum + Mistake[j] * Weights[j][i];
		}
		data[i] = sum;
		sum = 0;
	}
	return data;
}