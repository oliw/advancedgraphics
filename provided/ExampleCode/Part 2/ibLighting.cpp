#include "ibLighting.h"
#include <math.h>
#include <assert.h>

void createImage(float* rVectorsPFM, unsigned char* rVectorsPPM) {
  int width = 511;
  int height = 511;
  int components = 3;
  float r = 255;
  for (float x = -r; x < r; x++) {
    for (float y = -r; y < r; y++) {
      if (x*x + y*y <= r*r) {
        // calculate z
        float z = sqrt(r*r-x*x-y*y);
        // calculate polar coords
        float inclination = acos(z/r);
        float azimuth = atan(y/x);  
        // normalise
        float nx = x/255.0f;
        float ny = y/255.0f;
        float nz = z/255.0f;
        // calculate v
        float vx = x;
        float vy = y;
        float vz = -255 + z;
        float n = sqrt(vx*vx+vy*vy+vz*vz);
        vx /= n;
        vy /= n;
        vz /= n;
        // calculate n dot v
        float dot = nx*vx + ny*vy + nz*vz;
        // calculate r
        float rx = 2*dot*nx-vx;
        float ry = 2*dot*ny-vy;
        float rz = 2*dot*nz-vz;
        assert(rx >= -1 && rx <= 1);
        // encode r vector in a pfm
        rVectorsPFM[(int)((y+255)*width*components+(x+255)*components)] = rx;
        rVectorsPFM[(int)((y+255)*width*components+(x+255)*components) + 1] = ry;
        rVectorsPFM[(int)((y+255)*width*components+(x+255)*components) + 2] = rz;
        // encode r vector in a ppm
        rVectorsPPM[(int)((y+255)*width*components+(x+255)*components)] = ((rx+1)/2)*255;
        rVectorsPPM[(int)((y+255)*width*components+(x+255)*components) + 1] = ((ry+1)/2)*255;
        rVectorsPPM[(int)((y+255)*width*components+(x+255)*components) + 2] = ((rz+1)/2)*255;
      } else {
        // Not circle
        for (int i = 0; i < components; i++) {
          rVectorsPFM[(int)((y+255)*width*components+(x+255)*components) + i] = 0;
          rVectorsPPM[(int)((y+255)*width*components+(x+255)*components) + i] = 0;
        }
      }
    }
  }
}








