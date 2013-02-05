#include "ibLighting.h"
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>

using namespace std;

void normalise(float& x, float& y, float& z) {
    float n = sqrt(x*x+y*y+z*z);
    x /= n;
    y /= n;
    z /= n;
}

int toArrIndex(float x, float y) {
  return (int)((y+255)*511*3+(x+255)*3);
}

void createImage(float* rVectorsPFM, unsigned char* rVectorsPPM, float* mappedPFM, unsigned char* mappedPPM, float* texture) {
  float PI = atan(1.0f) * 4.0f;  
  int width = 511;
  int height = 511;
  int components = 3;
  float r = 255;
  for (float x = -r; x < r; x++) {
    for (float y = -r; y < r; y++) {
      if (x*x + y*y <= r*r) {
        float z = sqrt(r*r-x*x-y*y);
        // calculate z
        float sx = x;
        float sy = y;
        float sz = z;
        normalise(sx,sy,sz);
        // calculate polar coords
        float inclination = acos(z/r);
        float azimuth = atan2(y,x);  
        // calculate v
        float vx = -x;
        float vy = -y;
        float vz = -z + 255;
        normalise(vx,vy,vz);
        // calculate n dot v
        float dot = sx*vx + sy*vy + sz*vz;
        // calculate r
        float rx = 2*dot*sx-vx;
        float ry = 2*dot*sy-vy;
        float rz = 2*dot*sz-vz;
        normalise(rx,ry,rz);
        // encode r vector in a pfm
        rVectorsPFM[toArrIndex(x,y)] = rx;
        rVectorsPFM[toArrIndex(x,y) + 1] = ry;
        rVectorsPFM[toArrIndex(x,y) + 2] = rz;
        // encode r vector in a ppm
        rVectorsPPM[toArrIndex(x,y)] = ((rx+1)/2)*255;
        rVectorsPPM[toArrIndex(x,y) + 1] = ((ry+1)/2)*255;
        rVectorsPPM[toArrIndex(x,y) + 2] = ((rz+1)/2)*255;
        // shade using texture
        int texX = floor((((azimuth+PI)/(2*PI))*512)+0.5);
        int texY = floor((((inclination+(PI/2))/PI)*512)+0.5);
        for (int i = 0; i < components; i++) {
          float textureValue = min(1.0f, texture[texY*1024*3+texX*3+i]);
          //TODO PFM
          mappedPPM[(int)((y+255)*width*components+(x+255)*components)+i] = textureValue * 255;
        }
      } else {
        // Not circle
        for (int i = 0; i < components; i++) {
          rVectorsPFM[toArrIndex(x,y) + i] = 0;
          rVectorsPPM[toArrIndex(x,y) + i] = 0;
          mappedPFM[toArrIndex(x,y) + i] = 0;
          mappedPPM[toArrIndex(x,y) + i] = 0;
        }
      }
    }
  }
}








