#include <math.h>

float brightestPixel(unsigned int &imageSize, float* image) {
    float max = 0;
    for (int pixel = 0; pixel < imageSize; pixel++) {
        if (image[pixel] > max) {
          max = image[pixel];
        }
    }
    return max;
}

unsigned char* toPixelValues(unsigned int &imageSize, float* input) {
    unsigned char* output = new unsigned char[imageSize];
    for (int pixel = 0; pixel < imageSize; pixel++) {
        output[pixel] = input[pixel] * 255;
    }
    return output;
}
 
float* simpleToneMap(unsigned int &imageSize, float* input) {
    float max = brightestPixel(imageSize, input);
    for (int pixel = 0; pixel < imageSize; pixel++) {
        input[pixel] = input[pixel] / max;
    }
    return input;
}

float* adjustExposure(unsigned int &imageSize, float* input, unsigned int stops) {
    float* output = new float[imageSize];
    for (int pixel = 0; pixel < imageSize; pixel++) {
        output[pixel] = input[pixel] * pow(2, stops);
        if (output[pixel] > 1.0) {
          output[pixel] = 1.0;
        }
    }
    return output;
}

float* adjustGamma(unsigned int &imageSize, float* input, float gamma) {
    float* output = new float[imageSize];
    for (int pixel = 0; pixel < imageSize; pixel++) {
        output[pixel] = pow(input[pixel], 1/gamma);
        if (output[pixel] > 1.0) {
          output[pixel] = 1.0;
        }
    }
    return output;
}
