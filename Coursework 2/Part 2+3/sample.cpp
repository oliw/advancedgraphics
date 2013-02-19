#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>

#define PI 3.14159265358979323

using namespace std;

float* calculateIntensityMap(float *environmentMap) {
  float* intensityMap = new float[512 * 1024];
  for (int x = 0; x < 1024; x++) {
    for (int y = 0; y < 512; y++) {
      int index = (y * 1024 * 3) + (x * 3);
      float red = environmentMap[index];
      float green = environmentMap[index + 1];
      float blue  = environmentMap[index + 2];
      float theta = (y / 512.0) * PI;
      intensityMap[(y * 1024) + x] = sin(theta) * ((red + green + blue) / 3);
    }
  }
  return intensityMap;
}

float* calculateLineIntensities(float* intensityMap) {
  float* lineIntensities = new float[512];
  for (int y = 0; y < 512; y++) {
    lineIntensities[y] = 0;
    for (int x = 0; x < 1024; x++) {
      lineIntensities[y] += intensityMap[(y * 1024) + x];
    }
  }
  return lineIntensities;
}

float* calculateCDF(float *intensityMap, int pixels) {
  float* cdf = new float[pixels];
  float sum = 0;
  for (int i = 0; i < pixels; i++) {
    sum += intensityMap[i];
    cdf[i] = sum;
  }
  return cdf;
}

//Takes a CDF with 'size' query points
//Looks for sample points

//Tells you how many samples for the value

int* invertCDF(float* cdf, int size, int samples) {
  int *inversion = new int[size];
  float max = cdf[size - 1];
  float step = max / (samples + 1);
  float target = step;
  for (int i = 0; i < size; i++) {
    inversion[i] = 0;    
    while (target < cdf[i]) {
      float midpoint = cdf[i-1] + ((cdf[i] - cdf[i-1]) / 2);
      if (target <= midpoint) {
        inversion[i-1]++;
      } else {
        inversion[i]++;
      }
      target += step;
    }
  }
  return inversion;
}

void highlightPixel(float *sampleMap, int centerX, int centerY) {
  for (int x = centerX - 2; x < centerX + 2; x++) {
    for (int y = centerY - 2; y < centerY + 2; y++) {
      if (x >= 0 && x < 1024 && y < 512 && y >= 0) {
        int index = (y * 1024 * 3) + (x * 3);
        sampleMap[index + 0] = 0.0;
        sampleMap[index + 1] = 1.0;
        sampleMap[index + 2] = 0.0;  
      }
    }
  }
}

void sampleLine(float *intensityMap, float *sampledMap, int line, int samples) {
  float* cdf = calculateCDF(&intensityMap[line * 1024], 1024);
  int* xs = invertCDF(cdf, 1024, samples);
  for (int i = 0; i < 1024; i++) {
    if (xs[i] > 0) {
      highlightPixel(sampledMap, i, line);
    }
  }
}

void sampleEnvironmentMap(float *environmentMap, float *sampleMap, int samples) {
  float *intensityMap = calculateIntensityMap(environmentMap);
  cout << "Intensity Map" << endl;
  float *lineIntensities = calculateLineIntensities(intensityMap);
  cout << "Line intensities" << endl;
  float *cdf = calculateCDF(lineIntensities, 512);
  cout << "Cdf" << endl;
  int *samplesPerLine = invertCDF(cdf, 512, samples);
  cout << "Invert" << endl;
  for (int line = 0; line < 512; line++) {
    int lineSamples = samplesPerLine[line];
    if (lineSamples > 0) {
      sampleLine(intensityMap, sampleMap, line, lineSamples);
    }
  }
}

void samplePhongModel(float *sampleMap, int n, int samples) {
  for (int s = 0; s < samples; s++) {
    float u1 = ((float) rand() / (RAND_MAX));
    float u2 = ((float) rand() / (RAND_MAX));
    float theta = acos(pow((1-u1),(1.0f/(n+1))));
    assert(theta >= 0);
    float phi = 2*PI*u2;
    int centerX = u2*1024;
    int centerY = (theta/PI)*512;
    highlightPixel(sampleMap, centerX, centerY);
  }
}

