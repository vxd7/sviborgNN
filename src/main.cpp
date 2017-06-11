#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include "ConvNetwork.h"

int main() {

	ConvNetwork myNet("globConfig.cfg");

	std::string fname = "imgList.txt";
	std::cout << "ConvNetwork::getInput(std::string imageListFile)" << std::endl;
	myNet.getInput(fname);

	std::vector<double> out;
	const int mapNumber = 0;

	myNet.processInputMap(out, mapNumber);

	for(int i = 0; i < out.size(); ++i) {
		std::cout << out[i] << std::endl;
	}

	return 0;
}
