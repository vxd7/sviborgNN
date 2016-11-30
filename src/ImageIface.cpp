#include "ImageIface.h"

#include <iostream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
void ImageIface::getImageList(std::vector<std::string> imageList) {
	for(size_t i = 0; i < imageList.size(); ++i) {
		//if file exists
		//else -- throw ex
	}
	
}

void ImageIface::computeGrayscaleMatrix(std::string filename, std::vector <std::vector <double>> &imageMatrix) {
	int w, h, comp;

	unsigned char* map = stbi_load(filename.c_str(), &w, &h, &comp, 1);
	if(!map) {
		//throw ex
	}

	if(w * h == 0) {
		std::cout << "Error loading image! Probably image is not correct" << std::endl;
	}

	makeImageMatrix(imageMatrix, map, w, h);

	stbi_image_free(map);
}

void ImageIface::makeImageMatrix(std::vector<std::vector<double>> &matrix, unsigned char* map, int w, int h) {
	if((matrix.size() != h) || (matrix[0].size() != w)) {
		matrix.resize(h);
		for(size_t i = 0; i < h; ++i) {
			matrix[i].resize(w);
		}
	}

	/**
	 * i is for rows counting;
	 * j is for columns counting;
	 */
	matrix[0][0] = (double)map[0];
	for(size_t j = 1, i = 0; j < w*h; ++j) {

		if((j%w) == 0) {
			++i;
		}

		matrix[i][j%w] = (double)map[j];

	}
}

void ImageIface::computeGrayscaleMatrix(int number, std::vector <std::vector <double>> &imageMatrix) {
    computeGrayscaleMatrix(imgList[number], imageMatrix);
}
