#pragma once

#include <vector>

std::vector<double> operator -(std::vector<double> a, std::vector<double> b);
std::vector<double> operator +(std::vector<double> a, std::vector<double> b);
std::vector<double> operator *(std::vector<double> a, std::vector<double> b);

std::vector<std::vector<double>> operator +(std::vector<std::vector<double>> a, std::vector<std::vector<double>> b);

double Total(std::vector<double> a);