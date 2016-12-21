#include <ctime>
#include <fstream>
#include <sstream>
#include "MLP_InitializationFunctions.h"
std::vector<double> StringToVector(char buff[]);
std::vector<double> InitializationBias(int size)
{
	srand(time(NULL));
	std::vector<double> bias(size);
	for (int i = 0; i < size; i++)
	{
		bias[i] = (1 + rand() % 20000 - 10000) / 20000.0;
	}
	return bias;
}
void WritingBias(std::vector<double> bias, std::string filename)
{
	std::ofstream fout(filename);
	fout << bias[0];
	for (unsigned int i = 1; i < bias.size(); i++)
	{
		fout << ' ' << bias[i];
	}
	fout.close();
}
std::vector<double> ReadingBias(std::string filename)
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
std::vector<std::vector<double>> InitializationWeights(int ncols, int nrows)
{
	srand(time(NULL));
	std::vector<std::vector<double>> weights(nrows);
	for (int row = 0; row < nrows; ++row)
	{
		weights[row].resize(ncols);
	}
	for (int i = 0; i < nrows; ++i)
	{
		for (int j = 0; j < ncols; ++j)
		{
			weights[i][j] = (1 + rand() % 20000 - 10000) / 20000.0;
		}
	}
	return weights;
}
void WritingWeights(std::vector<std::vector<double>> weights, std::string filename)
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
std::vector<std::vector<double>> ReadingWeights(std::string filename)
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
std::vector<double> StringToVector(char buff[])
{
	std::vector<double> data;
	double number;
	std::stringstream ss(buff);
	while (ss >> number)
		data.push_back(number);
	return data;
}