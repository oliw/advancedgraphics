#ifndef __TONEMAPPER_H__
#define __TONEMAPPER_H__

unsigned char* simpleToneMap(unsigned int &imageSize, float* input);

float* adjustExposure(unsigned int &imageSize, float* input, unsigned int stops);

#endif
