#include "MLP_VectorFunctions.h"

std::vector<double> MLP::operator -(const std::vector<double> &a, const std::vector<double> &b)
{
	std::vector<double> c(a.size());
	for (unsigned int i = 0; i < c.size(); i++)
	{
		c[i] = a[i] - b[i];
	}
	return c;
}
std::vector<double> MLP::operator +(const std::vector<double> &a, const std::vector<double> &b)
{
	std::vector<double> c(a.size());
	for (unsigned int i = 0; i < c.size(); i++)
	{
		c[i] = a[i] + b[i];
	}
	return c;
}
std::vector<double> MLP::operator *(const std::vector<double> &a, const std::vector<double> &b)
{
	std::vector<double> c(a.size());
	for (unsigned int i = 0; i < c.size(); i++)
	{
		c[i] = a[i] * b[i];
	}
	return c;
}
std::vector<std::vector<double>> MLP::operator +(const std::vector<std::vector<double>> &a, const std::vector<std::vector<double>> &b)
{
	std::vector<std::vector<double>> c(a.size());
	for (unsigned int i = 0; i < c.size(); i++)
	{
		c[i].resize(a[0].size());
	}
	for (unsigned int i = 0; i < c.size(); i++)
	{
		for (unsigned int j = 0; j < c[0].size(); j++)
		{
			c[i][j] = a[i][j] + b[i][j];
		}
	}
	return c;
}
double MLP::Total(const std::vector<double> &a)
{
	double sum = 0;
	for (unsigned int i = 0; i < a.size(); i++)
	{
		sum = sum + a[i];
	}
	return sum;
}