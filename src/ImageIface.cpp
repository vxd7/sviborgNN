#include "ImageIface.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define NORMCOEFF 255.0

//https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
void ImageIface::getImageList(std::vector<std::string> &imageList) {
	std::ifstream map;
	const char* fn = "testpic.jpg";
	map.open(fn, std::ios::in | std::ios::binary);
     
    for (size_t i = 0; i < imageList.size(); ++i) { 
		//map.open(imageList[i].c_str(), std::ios::in | std::ios::binary);
        try {
           
            map.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            imgList.push_back(imageList[i]);
            
        }
        catch (const std::ifstream::failure& e) {
            std::cerr << "Exception opening/reading file -- index " << i + 1;
        }
       
    }
   map.close();
}

void ImageIface::computeGrayscaleMatrix(std::string filename, std::vector <std::vector <double>> &imageMatrix) {
	int w, h, comp;

	unsigned char* map = stbi_load(filename.c_str(), &w, &h, &comp, 1);
    try {
        if (!map) {
            throw (filename);
        }
    }
    catch (std::string a) {
        std::cerr << "Exception opening image: " << filename;
    }
	if(w * h == 0) {
		std::cout << "Error loading image! Probably image is not correct" << std::endl;
	}
    
	makeImageMatrix(imageMatrix, map, w, h);
    if (w < w_max || h < h_max) {
        normalizeImageMatrix(imageMatrix);
    }
	// normalization
	for (int i = 0; i < imageMatrix.size(); ++i) {
		for (int j = 0; j < imageMatrix[i].size(); ++j) {
			imageMatrix[i][j] /= NORMCOEFF;
		}
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
        try {
            if (!map) {
                throw (imgList[i]);
            }
        }
        catch (std::string filename) {
            std::cerr << "Exception opening image: " << filename;
        }
        if (w > w_max) { w_max = w; }
        if (h > h_max) { h_max = h; }

		stbi_image_free(map);
    }
}
void ImageIface::normalizeImageMatrix(std::vector<std::vector<double>> &imageMatrix) {
    int h = imageMatrix.size();
    int w = imageMatrix[0].size();
    std::vector <double> zero(w_max);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w_max - w; ++j) {
            imageMatrix[i].push_back(0.0);
        }
    }
    for (int i = h; i < h_max; ++i) {
        imageMatrix.push_back(zero);
    }
}
