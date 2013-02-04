#ifndef __ASSEMBLEHDR_H__
#define __ASSEMBLEHDR_H__

void assembleHDR(unsigned int& width, 
                  unsigned int &height,
	                unsigned int &numComponents,
                  unsigned int &numImages, 
                  float** images,
                  float* output);

float calculateRange(float* image,
                  unsigned int& width, 
                  unsigned int &height,
	                unsigned int &numComponents);

#endif
