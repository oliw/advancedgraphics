#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>

using namespace std;

float PI = atan(1.0f) * 4.0f;
float emR = 1;

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
  float azimuth = (texX / 1024.0) * 2 * PI;
  float inclination = (texY / 512.0) * PI;
  x = emR * sin(inclination) * sin(azimuth);
  y = emR * cos(inclination);
  z = emR * sin(inclination) * cos(azimuth);
}

void renderPixel(float* rendering, float* sampleMap, int samples, int x, int y) {
  float z = sqrt(255*255 - x*x - y*y);
  // calculate normal
  float nx = x;
  float ny = y;
  float nz = z;
  normalise(nx, ny, nz);
  // calculate reflected
  float rx = nx - 0;
  float ry = ny - 0;
  float rz = nz - 1;
  normalise(rx, ry, rz);
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
  incidentSum /= samples;
  int index = toArrIndex(x, y);
  rendering[index + 0] = redAngleSum;
  rendering[index + 1] = greenAngleSum;
  rendering[index + 2] = blueAngleSum;
}

void render(float* rendering, float* sampleMap, int samples) {
  for (float x = -255; x < 255; x++) {
    for (float y = -255; y < 255; y++) {
      if (x * x + y * y <= 255 * 255) {
        renderPixel(rendering, sampleMap, samples, x, y);
      } else {
        rendering[toArrIndex(x, y) + 0] = 0;
        rendering[toArrIndex(x, y) + 1] = 0;
        rendering[toArrIndex(x, y) + 2] = 0;
      }
    }
  }
}
