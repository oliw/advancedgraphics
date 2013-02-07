#include "assembleHDR.h"
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <limits>

using namespace std;

float weighting(float x, float max, float min) {
  if (x <= 0.5*(min+max)) {
    return x - min;
  } else {
    return max - x;
  }
//  if (x < 0.5) {
//    return x*2;
//  } else {
//    return x*-2 + 2;
//  }
  //return (1.0 / (0.16 * sqrt(2.0 * PI))) * exp(-pow(x - 0.5, 2.0)/(2.0 * pow(0.16, 2.0))) / 2.5;
}

float * calculateEi() {
    int numImages = 7;
    float* ei = new float [numImages];
    float currentEi = 1;
    for (int i = 0; i < numImages; i++) {
        ei[i] = currentEi;
        currentEi *= 4;
    }
    return ei;
}

bool withinThreshold(float value) {
  float max = 0.92;
  float min = 0.005;
  return value > min && value < max;
}

void calculateMinsAndMaxs(float** minValues, 
                          float** maxValues,
                          float** images, 
                          unsigned int& numImages, 
                          unsigned int& imageSize,
                          unsigned int& numComponents)
{
  for (unsigned int i = 0; i < numImages; i++) {
    minValues[i] = new float[numComponents];
    maxValues[i] = new float[numComponents];
    for (int compI = 0; compI < numComponents; compI++) {
      minValues[i][compI] = 1.0f;
      maxValues[i][compI] = 0.0f;
    }
    for (int pixel = 0; pixel < imageSize*numComponents; pixel+=numComponents) {
      for (int comp = pixel; comp < pixel+numComponents; comp++) {
        int compIndex = comp - pixel;
        float pixelValue = images[i][comp];
        if (pixelValue > maxValues[i][compIndex]) {
          maxValues[i][compIndex] = pixelValue;
        }
        if (pixelValue < minValues[i][compIndex]) {
          minValues[i][compIndex] = pixelValue;
        }
      }
    }
  //cout << "Image: " << i << " ";
  //  for (int compI = 0; compI < numComponents; compI++) {
  //    cout << "Component:" << compI << " ";       
  //    cout << "Min:" << minValues[i][compI] << " ";
  //    cout << "Max:" << maxValues[i][compI] << " ";
  //cout << endl;
  }
}    

float calculateRange(float* image,
                  unsigned int &width, 
                  unsigned int &height,
	                unsigned int &numComponents) {
    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();
    for (int w = 0; w < width; w++) {
      for (int h = 0; h < height; h++) {
        int curr = h*width + w;
        float pixelValue = 0;
        for (int i = 0; i < numComponents; i++) {
          pixelValue += image[curr+i];
        }    
        pixelValue /= numComponents;
        if (pixelValue > max) {
          max = pixelValue;
        }
        if (pixelValue < min) {
          min = pixelValue;
        }
      }
    }
    return max / min;
}

void assembleHDR(unsigned int& width, 
                  unsigned int &height,
	                unsigned int &numComponents,
                  unsigned int &numImages, 
                  float** images,
                  float* output) {
    assert(numImages == 7);
    // Compute the relative exposure length for each image
    float* ei = calculateEi();
    float** minValues = new float*[numImages];
    float** maxValues = new float*[numImages];
    unsigned int imageSize = width*height;
    // Needed for weighting function (see paper)
    calculateMinsAndMaxs(minValues,maxValues,images, numImages, imageSize,numComponents);    
    for (int x = 0; x < width; x++) {
      for (int y = 0; y < height; y++) {
        for (int c = 0; c < numComponents; c++) {
          int current = y*width*numComponents + x*numComponents + c; 
          float enumerator = 0;
          float denominator = 0;
          for (int i = 0; i < numImages; i++) {
            float pixelValue = images[i][current];
            if (withinThreshold(pixelValue)) {
                float weight = weighting(pixelValue,maxValues[i][c],minValues[i][c]);
                // Calculate enumerator
                enumerator += log(1.0f/ei[i] * pixelValue) * weight;
                // Calculate denomiator
                denominator += weight;
            }
          }
          output[current] = exp(enumerator/denominator);
        }
      }
    }
}
