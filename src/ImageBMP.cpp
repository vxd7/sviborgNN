#include "EasyBMP.h"
#include "ImageBMP.h"
#include <vector>
using namespace std;

//ImageBMP::ImageBMP() {
//    
//    //try {}
//   /* inputImage.ReadFromFile(filename);
//    TurnToGrey();
//   */
//}
void ImageBMP::TurnToGrey() {

    for (int i = 0; i < inputImage.TellWidth(); i++)
    {
        for (int j = 0; j < inputImage.TellHeight(); j++)
        {
            double Temp = 0.30*(inputImage(i, j)->Red) +
                0.59*(inputImage(i, j)->Green) +
                0.11*(inputImage(i, j)->Blue);
            inputImage(i, j)->Red = (ebmpBYTE)Temp;
            inputImage(i, j)->Green = (ebmpBYTE)Temp;
            inputImage(i, j)->Blue = (ebmpBYTE)Temp;
        }
    }

    inputImage.SetBitDepth(8);

    // Create a greyscale color table
    CreateGrayscaleColorTable(inputImage);
    inputImage.WriteToFile("output.bmp");
}
void ImageBMP::ComputeMatrix(const char* filename, std::vector <std::vector <double>> &imageMatrix)
{
    //try{}
    inputImage.ReadFromFile(filename);
    TurnToGrey();
    imageMatrix.resize(inputImage.TellHeight());
    for (int i = 0; i < inputImage.TellHeight(); ++i) {
        imageMatrix[i].resize(inputImage.TellWidth());
    }
    for (int i = 0; i < inputImage.TellHeight(); ++i) {
      for (int j = 0; j < inputImage.TellWidth(); ++j) {
        imageMatrix[i][j] = (double) inputImage(i-1,j-1) ->Red;
      }
    }

  
}

