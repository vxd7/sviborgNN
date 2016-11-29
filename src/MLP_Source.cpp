#include "MLP_Network.h"
void main()
{
	std::vector<double> input(5);
	input[0] = 0.1;
	input[1] = 0.2;
	input[2] = 0.3;
	input[3] = 0.4;
	input[4] = 0.5;
	std::vector<double> teacher(5);
	teacher[0] = 0.2;
	teacher[1] = 0.4;
	teacher[2] = 0.6;
	teacher[3] = 0.8;
	teacher[4] = 1.0;
	//std::vector<int> n(5);
	//n[0] = 5;
	//n[1] = 5;
	//n[2] = 5;
	//n[3] = 5;
	//n[4] = 5;
	//Network c(n);
	Network c("C:\\Users\\muha4\\Desktop\\NW\\NW\\NW", 4);
	c.NetChain(input);
	c.NetTrain(input, teacher, 0.5);
}