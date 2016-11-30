#include "ImageIface.h"

#include <iostream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "C:\Users\Gleb\Desktop\stb_image.h"

//https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
void ImageIface::getImageList(std::vector<std::string> &imageList) {
	for(size_t i = 0; i < imageList.size(); ++i) {
		//if file exists
		//else -- throw ex
        std::ifstream map(imageList[i]);
        if (map.good()) {
            imgList.push_back(imageList[i]);
        }
        else
            throw ; 
        map.close();
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
    if (w < w_max || h < h_max) {
        normalizeImageMatrix(imageMatrix);
    }
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
void ImageIface::normalizeEverything() {
    h_max = w_max = 0;
    for (int i = 0; i < imgList.size(); ++i) {
        int w, h, comp;
        unsigned char* map = stbi_load(imgList[i].c_str(), &w, &h, &comp, 1);
        if (!map) {
            //throw ex
        }
        if (w > w_max) { w_max = w; }
        if (h > h_max) { h_max = h; }
    }
}
void ImageIface::normalizeImageMatrix(std::vector<std::vector<double>> &imageMatrix) {
    int h = imageMatrix.size();
    int w = imageMatrix[0].size();
    std::vector <double> zero(w_max);
    for (int i = 0; i < h; ++i) {
        for (int j = w; j < w_max - w; ++j) {
            imageMatrix[i].push_back(0.0);
        }
    }
    for (int i = h; i < h_max; ++i) {
        imageMatrix.push_back(zero);
    }
}
