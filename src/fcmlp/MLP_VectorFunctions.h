#pragma once

#include <vector>

namespace MLP
{
	std::vector<double> operator -(const std::vector<double> &a, const std::vector<double> &b);
	std::vector<double> operator +(const std::vector<double> &a, const std::vector<double> &b);
	std::vector<double> operator *(const std::vector<double> &a, const std::vector<double> &b);

	std::vector<std::vector<double>> operator +(const std::vector<std::vector<double>> &a, const std::vector<std::vector<double>> &b);

	double Total(const std::vector<double> &a);
}