#pragma once

#include <vector>
#include <string>

namespace MLP
{
	std::vector<double> StringToVector(char buff[]);

	std::vector<double> InitializationBias(const int &size);
	void WritingBias(const std::vector<double> &bias, const std::string &filename);
	std::vector<double> ReadingBias(const std::string &filename);

	std::vector<std::vector<double>> InitializationWeights(const int &ncols, const int &nrows);
	void WritingWeights(const std::vector<std::vector<double>> &weights, const std::string &filename);
	std::vector<std::vector<double>> ReadingWeights(const std::string &filename);
}