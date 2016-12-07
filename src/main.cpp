#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include "ConvNetwork.h"

int main() {
	std::vector <std::pair<int, int>> dims;
	std::vector<int> nl;

	nl.resize(4);
	dims.resize(4);

	nl[0] = 3; //C1
	nl[1] = 3; //S2
	nl[2] = 4; //C3
	nl[3] = 4; //S4

	dims[0] = std::make_pair(3,3);
	dims[1] = std::make_pair(2,2);
	dims[2] = std::make_pair(1,1);
	dims[3] = std::make_pair(2,2);

	std::vector <double> netOut;

	ConvNetwork myNet(nl, dims);

	std::string fname = "./imgList.txt";
	myNet.getInput(fname);
	myNet.processInputMap(netOut);

	std::cout << netOut.size() << std::endl;
	for(size_t i = 0; i < netOut.size(); ++i) {
		std::cout << netOut[i];
	}

	return 0;
}
