#include "MLP_Network.h"
#include <iostream>
void main()
{

	std::vector<int> n(3);
	n[0] = 2;
	n[1] = 32;
	n[2] = 1;
	Network d(n);
	Network c("C:\\Users\\muha4\\Desktop\\NW\\NW\\NW", 2);

	std::vector<std::vector<double>> a(231);
	for (int i = 0; i < 231; i++)
	{
		a[i].resize(2);
	}
	std::vector<std::vector<double>> b(231);
	for (int i = 0; i < 231; i++)
	{
		b[i].resize(1);
	}
	for (int i = 0; i < 11; i++)
	{
		for (int j = 21 * i; j < 21 * (i + 1); j++)
		{
			a[j][0] = 0.1*i - 1.0;
			a[j][1] = 0.1*(j - 21 * i) - 1.0;
			b[j][0] = a[j][0] * a[j][1];
		}
	}

	for (int i = 0; i < 231; i++)
	{
		c.NetTrain(a[i], b[i], 0.1);
		c.NetChain(a[i]);
		c.PrintOutput();
	}

	std::cout << std::endl;

	std::vector<double> input1(2);
	input1[0] = -0.3;
	input1[1] = 0.4;

	std::vector<double> input2(2);
	input2[0] = 0;
	input2[1] = 0;

	std::vector<double> input3(2);
	input3[0] = 0.9;
	input3[1] = 0.9;
	c.NetChain(input1);
	c.PrintOutput();
	c.NetChain(input2);
	c.PrintOutput();
	c.NetChain(input3);
	c.PrintOutput();
}