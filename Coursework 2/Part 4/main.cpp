/*//////////////////////////////////////////////////////////////////////////
Author: Abhijeet Ghosh
Year: 2013
//////////////////////////////////////////////////////////////////////////*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "loadPNM.h"
#include "sample.h"
#include "render.h"
#include "toneMapper.h"

//#define PI 3.14159265358979323
#define uint unsigned int

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

unsigned int width;
unsigned int height;
unsigned int numComponents;

void CreateAndSavePFM(const char *image_out)
{
	width = 511; // set size of image to 511x511 pixels
	height = 511;
	numComponents = 3;
	
	float *img_out = new float [width*height*numComponents];

	for ( uint i = 0 ; i < height ; ++i ) // height
    {
		for ( uint j = 0 ; j < width ; ++j ) // width
		{
			for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
			{
				uint index = i*width*numComponents + j*numComponents + k; //index within the image

				//set image to white
				img_out[index] = 1.0f; //R
				img_out[index + 1] = 1.0f;//G
				img_out[index + 2] = 1.0f;//B
				
			}

		}
	}


	WritePFM(image_out, width, height, numComponents, img_out);

}

void LoadAndSavePPM(const char *image_in, const char *image_out)
{
	unsigned char *img_in = loadPNM(image_in, width, height, numComponents);
	unsigned char *img_out = new unsigned char [width*height*numComponents];

	for ( uint i = 0 ; i < height ; ++i ) // height
    {
		for ( uint j = 0 ; j < width ; ++j ) // width
		{
			for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
			{
				uint index = i*width*numComponents + j*numComponents + k; //index within the image

				img_out[index] = img_in[index]; //R
				img_out[index + 1] = img_in[index + 1];//G
				img_out[index + 2] = img_in[index + 2];//B
				
			}

		}
	}


	WritePNM(image_out, width, height, numComponents, img_out);

}

void LoadAndSavePFM(const char *image_in, const char *image_out)
{
	float *img_in = loadPFM(image_in, width, height, numComponents);
	float *img_out = new float [width*height*numComponents];

	for ( uint i = 0 ; i < height ; ++i ) // height
    {
		for ( uint j = 0 ; j < width ; ++j ) // width
		{
			for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
			{
				uint index = i*width*numComponents + j*numComponents + k; //index within the image

				img_out[index] = img_in[index]; //R
				img_out[index + 1] = img_in[index + 1];//G
				img_out[index + 2] = img_in[index + 2];//B
				
			}

		}
	}


	WritePFM(image_out, width, height, numComponents, img_out);

}

void LoadPPMAndSavePFM(const char *image_in, const char *image_out)
{
	unsigned char *img_in = loadPNM(image_in, width, height, numComponents);
	float *img_out = new float [width*height*numComponents];

	for ( uint i = 0 ; i < height ; ++i ) // height
    {
		for ( uint j = 0 ; j < width ; ++j ) // width
		{
			for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
			{
				uint index = i*width*numComponents + j*numComponents + k; //index within the image

				//typecast 0 - 255 values to the 0.0f -> 1.0f range 
				img_out[index] = static_cast<float>(img_in[index])/255.0f; //R
				img_out[index + 1] = static_cast<float>(img_in[index + 1])/255.0f;//G
				img_out[index + 2] = static_cast<float>(img_in[index + 2])/255.0f;//B
				
			}

		}
	}


	WritePFM(image_out, width, height, numComponents, img_out);

}

void LoadPFMAndSavePPM(const char *image_in, const char *image_out)
{
	float *img_in = loadPFM(image_in, width, height, numComponents);
	unsigned char *img_out = new unsigned char [width*height*numComponents];

	for ( uint i = 0 ; i < height ; ++i ) // height
    {
		for ( uint j = 0 ; j < width ; ++j ) // width
		{
			for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
			{
				uint index = i*width*numComponents + j*numComponents + k; //index within the image
				
				//typecast 0.0f -> 1.0f values to the 0 - 255 range 
				img_out[index] = static_cast<unsigned char>(img_in[index]*255.0f); //R
				img_out[index + 1] = static_cast<unsigned char>(img_in[index + 1]*255.0f);//G
				img_out[index + 2] = static_cast<unsigned char>(img_in[index + 2]*255.0f);//B
				
			}

		}
	}

	WritePNM(image_out, width, height, numComponents, img_out);

}

int main(int argc, char** argv) {
  unsigned int width = 1024;
  unsigned int height = 512;
  unsigned int renderSize = 511;
  unsigned int channels = 3;
  unsigned int imageSize = width * height * channels;
  int samples [3] = {64, 256, 1024};
  for (int s = 0; s < 3; s++) {
    int sampleSize = samples[s];
    cout << "Rendering with " << sampleSize << " samples" << endl;
    float* environmentMapPFM = loadPFM("GraceCathedral/grace_latlong.pfm", width, height, channels);
    float* sampleMap = new float[5 * (sampleSize + 1)];
    sampleEnvironmentMap(environmentMapPFM, sampleMap, sampleSize);
    float* renderingPFM = new float[renderSize * renderSize * channels];
    render(renderingPFM, sampleMap, sampleSize);
    // Write PFM
    stringstream s2;
    s2 << "rendering_" << sampleSize << ".pfm";
    WritePFM(s2.str().c_str(), renderSize, renderSize, channels, renderingPFM);
    // Write PPM
    renderingPFM = simpleToneMap(imageSize, renderingPFM);
    //renderingPFM = adjustExposure(imageSize, renderingPFM, 2);
    renderingPFM = adjustGamma(imageSize, renderingPFM, 2.2);  
    unsigned char *renderingPPM = toPixelValues(imageSize, renderingPFM);
    stringstream s1;
    s1 << "rendering_" << sampleSize << ".ppm";
    WritePNM(s1.str().c_str(), renderSize, renderSize, channels, renderingPPM);
  }
  return 0;
}


