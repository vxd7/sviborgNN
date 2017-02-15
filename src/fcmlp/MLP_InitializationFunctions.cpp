#include <random>
#include <fstream>
#include <sstream>
#include "MLP_InitializationFunctions.h"

std::vector<double> MLP::InitializationBias(const int &size)
{
	std::mt19937 gen(std::random_device().operator()());
	std::uniform_real_distribution<> urd(-0.5, 0.5);
	std::vector<double> bias(size);
	for (int i = 0; i < size; i++)
	{
		bias[i] = urd(gen);
	}
	return bias;
}
void MLP::WritingBias(const std::vector<double> &bias, const std::string &filename)
{
	std::ofstream fout(filename);
	fout << bias[0];
	for (unsigned int i = 1; i < bias.size(); i++)
	{
		fout << ' ' << bias[i];
	}
	fout.close();
}
std::vector<double> MLP::ReadingBias(const std::string &filename)
{
	std::ifstream file(filename);
	char buff[1024];
	file.getline(buff, 1024);
	std::stringstream ss(buff);
	double number;
	std::vector<double> data;
	while (ss >> number)
		data.push_back(number);
	file.close();
	return data;
}
std::vector<std::vector<double>> MLP::InitializationWeights(const int &ncols, const int &nrows)
{
	std::mt19937 gen(std::random_device().operator()());
	std::uniform_real_distribution<> urd(-0.5, 0.5);
	std::vector<std::vector<double>> weights(nrows);
	for (int row = 0; row < nrows; ++row)
	{
		weights[row].resize(ncols);
	}
	for (int i = 0; i < nrows; ++i)
	{
		for (int j = 0; j < ncols; ++j)
		{
			weights[i][j] = urd(gen);
		}
	}
	return weights;
}
void MLP::WritingWeights(const std::vector<std::vector<double>> &weights, const std::string &filename)
{
	std::ofstream fout(filename);
	fout << weights[0][0];
	for (unsigned int j = 1; j < weights[0].size(); j++)
	{
		fout << ' ' << weights[0][j];
	}
	for (unsigned int i = 1; i < weights.size(); i++)
	{
		fout << std::endl;
		fout << weights[i][0];
		for (unsigned int j = 1; j < weights[0].size(); j++)
		{
			fout << ' ' << weights[i][j];
		}
	}
}
std::vector<std::vector<double>> MLP::ReadingWeights(const std::string &filename)
{
	std::ifstream file(filename);
	char buff[1024];
	std::vector<std::vector<double>> data;
	while (!file.eof())
	{
		file.getline(buff, 1024, '\n');
		data.push_back(StringToVector(buff));
	}
	return data;
}
std::vector<double> MLP::StringToVector(char buff[])
{
	std::vector<double> data;
	double number;
	std::stringstream ss(buff);
	while (ss >> number)
		data.push_back(number);
	return data;
}