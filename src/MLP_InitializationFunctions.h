#pragma once

#include <vector>
#include <string>

std::vector<double> InitializationBias(int size);
void WritingBias(std::vector<double> bias, std::string filename);
std::vector<double> ReadingBias(std::string filename);

std::vector<std::vector<double>> InitializationWeights(int ncols, int nrows);
void WritingWeights(std::vector<std::vector<double>> weights, std::string filename);
std::vector<std::vector<double>> ReadingWeights(std::string filename);