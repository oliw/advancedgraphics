#include "assembleHDR.h"
#include <math.h>
#include <assert.h>

float weighting(float x) {
  if (x < 0.5) {
    return x*2;
  } else {
    return x*-2 + 2;
  }
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

void assembleHDR(unsigned int& width, 
                  unsigned int &height,
	                unsigned int &numComponents,
                  unsigned int &numImages, 
                  float** images,
                  float* output) {

    assert(numImages == 7);
    // Assumed linear response curve
    // Relative exposures are 1,4,16,64,256,1024,4096

    // Compute the relative exposure for each image
    float* ei = calculateEi();    
    for (int x = 0; x < width; x++) {
      for (int y = 0; y < height; y++) {
        for (int c = 0; c < numComponents; c++) {
          int current = x*y*c;
          // Calculate output[current]
          float value = 0;
          float enumerator = 0;
          float denominator = 0;
          for (int i = 0; i < numImages; i++) {
            float weight = weighting(images[i][current]);
            // Calculate enumerator
            enumerator += log(1/ei[i] * images[i][current]) * weight;
            // Calculate denomiator
            denominator += weight;
          }
          output[current] = exp(enumerator/denominator);
        }
      }
    }
}
