#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "sample.h"
#include <assert.h>

using namespace std;

float PI = atan(1.0f) * 4.0f;

void normalise(float& x, float& y, float& z) {
  float n = sqrt(x*x+y*y+z*z);
  x /= n;
  y /= n;
  z /= n;
}

int toArrIndex(float x, float y) {
  return (int)((y + 255) * 511 * 3 + (x + 255) * 3);
}

void environmentMapToGeometric(int texX, int texY, float& x, float& y, float& z) {
  float azimuth = ((texX / 1024.0) * 2 * PI) - PI;
  float inclination = ((texY / 512.0) * PI) - PI;
  x = sin(inclination) * sin(-azimuth);
  y = cos(inclination);
  z = sin(inclination) * cos(-azimuth);
}

void renderPixel(float* rendering, float* sampleMap, int samples, int x, int y, float averageIntensity) {
  float z = sqrt(255*255 - x*x - y*y);
  // calculate normal
  float nx = x;
  float ny = y;
  float nz = z;
  normalise(nx, ny, nz);
  // calculate pixel value
  float incidentSum = 0;
  float redAngleSum = 0;
  float greenAngleSum = 0;
  float blueAngleSum = 0; 
  for (int index = 0; index < samples; index++) {
    float x = sampleMap[index * 5 + 0];
    float y = sampleMap[index * 5 + 1];
    float r = sampleMap[index * 5 + 2];
    float g = sampleMap[index * 5 + 3];
    float b = sampleMap[index * 5 + 4];
    incidentSum += (r + g + b) / 3;
    float emX;
    float emY;
    float emZ;
    environmentMapToGeometric(x, y, emX, emY, emZ);
    normalise(emX, emY, emZ);
    // Cosine of angle between normal and incident
    float cosine = emX * nx + emY * ny + emZ * nz;
    float n = sqrt(r*r + g*g + b*b);
    float commonAngleSum = (1 / PI) * cosine;
    if (cosine > 0) {
      redAngleSum += commonAngleSum * r/n;
      greenAngleSum += commonAngleSum * g/n;
      blueAngleSum += commonAngleSum * b/n;
    }
  }
  averageIntensity /= samples;
  int index = toArrIndex(x, y);
  rendering[index + 0] = averageIntensity * redAngleSum;
  rendering[index + 1] = averageIntensity * greenAngleSum;
  rendering[index + 2] = averageIntensity * blueAngleSum;
}

void render(float* rendering, float* sampleMap, float* environmentMap, int samples) {
  float averageIntensity = calculateAverageIntensity(environmentMap);
  for (float x = -255; x < 255; x++) {
    for (float y = -255; y < 255; y++) {
      if (x * x + y * y <= 255 * 255) {
        renderPixel(rendering, sampleMap, samples, x, y, averageIntensity);
      } else {
        rendering[toArrIndex(x, y) + 0] = 0;
        rendering[toArrIndex(x, y) + 1] = 0;
        rendering[toArrIndex(x, y) + 2] = 0;
      }
    }
  }
}
