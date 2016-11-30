#pragma once

#include <fstream>
#include <string>
#include <vector>


class ImageIface {
public:
	void getImageList(std::vector<std::string> imageList);
	void normalizeEverything();

    void computeGrayscaleMatrix(std::string filename, std::vector <std::vector <double>> &imageMatrix);
    void computeGrayscaleMatrix(int number, std::vector <std::vector <double>> &imageMatrix);
private:
	std::vector<std::string> imgList;
	void makeImageMatrix(std::vector<std::vector<double>> &matrix, unsigned char* map, int w, int h);

	void normalizeImageMatrix(std::vector<std::vector<double>>, int w, int h);
};
