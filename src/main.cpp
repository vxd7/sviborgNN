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
	
	std::cout << "ConvNetwork::processInputMap(std::vector <double> &outputMap) " << std::endl;

	return 0;
}
