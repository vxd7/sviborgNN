#pragma once

#include "EasyBMP.h"
#include <vector>
using namespace std;
class ImageBMP {
public:
    //ImageBMP();
    void TurnToGrey();
    void ComputeMatrix(const char* filename, std::vector <std::vector <double>> &imageMatrix);
    //~ImageBMP();

private:
    BMP inputImage;
};
