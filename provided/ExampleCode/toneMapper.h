#ifndef __TONEMAPPER_H__
#define __TONEMAPPER_H__

unsigned char* toPixelValues(unsigned int &imageSize, float* input);

float* simpleToneMap(unsigned int &imageSize, float* input);

float* adjustExposure(unsigned int &imageSize, float* input, unsigned int stops);

float* adjustGamma(unsigned int &imageSize, float* input, float gamma);

#endif
