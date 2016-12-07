#include "MLP_Network.h"
void main()
{
	std::vector<double> input(2);
	input[0] = -0.3;
	input[1] = 0.4;
	std::vector<double> teacher(1);
	teacher[0] = -0.12;
	std::vector<int> n(3);
	n[0] = 2;
	n[1] = 4;
	n[2] = 1;
	//Network d(n);
	Network c("C:\\Users\\muha4\\Desktop\\NW\\NW\\NW", 2);
	c.NetChain(input);
	c.PrintOutput();
	c.NetTrain(input, teacher, 0.1);
	c.NetChain(input);
	c.PrintOutput();
}