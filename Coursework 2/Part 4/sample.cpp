#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>

#define PI 3.14159265358979323

using namespace std;

int sample = 0;

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

void sampleLine(float *environmentMap, float *intensityMap, float *sampleMap, int line, int samples) {
  float* cdf = calculateCDF(&intensityMap[line * 1024], 1024);
  int* xs = invertCDF(cdf, 1024, samples);
  for (int i = 0; i < 1024; i++) {
    if (xs[i] > 0) {
      int index = (line * 1024 * 3) + (i * 3);
      sampleMap[sample * 5 + 0] = i;
      sampleMap[sample * 5 + 1] = line;
      sampleMap[sample * 5 + 2] = environmentMap[index + 0];
      sampleMap[sample * 5 + 3] = environmentMap[index + 1];
      sampleMap[sample * 5 + 4] = environmentMap[index + 2];
      sample++;
    }
  }
}

void sampleEnvironmentMap(float *environmentMap, float *sampleMap, int samples) {
  sample = 0;
  float *intensityMap = calculateIntensityMap(environmentMap);
  float *lineIntensities = calculateLineIntensities(intensityMap);
  float *cdf = calculateCDF(lineIntensities, 512);
  int *samplesPerLine = invertCDF(cdf, 512, samples);
  for (int line = 0; line < 512; line++) {
    int lineSamples = samplesPerLine[line];
    if (lineSamples > 0) {
      sampleLine(environmentMap, intensityMap, sampleMap, line, lineSamples);
    }
  }
}
