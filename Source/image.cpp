#include "image.h"
#include "bmp.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>


/**
 * Image
 **/
Image::Image (int width_, int height_)
{
    assert(width_ > 0);
    assert(height_ > 0);

    width           = width_;
    height          = height_;
    num_pixels      = width * height;
    pixels          = new Pixel[num_pixels];
    sampling_method = IMAGE_SAMPLING_POINT;

    assert(pixels != NULL);
}


Image::Image (const Image& src)
{
    width           = src.width;
    height          = src.height;
    num_pixels      = width * height;
    pixels          = new Pixel[num_pixels];
    sampling_method = IMAGE_SAMPLING_POINT;

    assert(pixels != NULL);
    memcpy(pixels, src.pixels, src.width * src.height * sizeof(Pixel));
}


Image::~Image ()
{
    delete [] pixels;
    pixels = NULL;
}

/*
void Image::AddNoise (double factor)
{

}
*/

/**
* Brightens the image by lerping it with black,
* using the input, factor, as the parameter
*/
void Image::Brighten (double factor)
{
	// factor should be non-negative
	if (factor < 0)
		return;

	Pixel p = Pixel();

	for (int i = 0; i < num_pixels; i++) 
		pixels[i] = PixelLerp(p, pixels[i], factor);
}

/**
* Changes the contrast in the image by lerping it with a shade of gray,
* using the input, factor, as the parameter.
* The shade of gray is determined using the average luminance of the image.
*/
void Image::ChangeContrast (double factor)
{
	// caluculate the average luminance of the image
	double avg = 0;
	for (int i = 0; i < num_pixels; i++)
		avg += pixels[i].Luminance() * 1.0 / num_pixels;
	
	Pixel p = Pixel();
	p.SetClamp(avg, avg, avg);

	for (int i = 0; i < num_pixels; i++)
		pixels[i] = PixelLerp(p, pixels[i], factor);
}

/**
* Changes the saturation of the image by lerping it with a grayscale version,
* using the input, factor, as the parameter.
* The grayscale version consists of the luminance f the original at each pixel.
*/
void Image::ChangeSaturation(double factor)
{
	Pixel p = Pixel();
	for (int i = 0; i < num_pixels; i++) {
		Component lum = pixels[i].Luminance();
		p.Set(lum, lum, lum);
		pixels[i] = PixelLerp(p, pixels[i], factor);
	}
}

/**
 * Applies gamma correction to image using the formula:
 * Cn = Co^(1/factor), where factor > 0 
 */
void Image::ChangeGamma(double factor)
{
	// gamma factor should be positive
	if (factor <= 0)
		return;

	// apply gamma correction to pixels
	for (int i = 0; i < num_pixels; i++) {
		double exp = 1.0 / factor;
		pixels[i].SetClamp(
			pow(pixels[i].r, exp), 
			pow(pixels[i].g, exp), 
			pow(pixels[i].b, exp));
	}
}

Image* Image::Crop(int x, int y, int w, int h)
{
	// all parameters must be positive
	if (x < 0 || y < 0 || w < 0 || h < 0)
		return NULL;
  return NULL ;
}


void Image::ExtractChannel(int channel)
{
    // For extracting a channel (R,G,B) of image.  
    // Not required for the assignment
    for(int i = 0; i < num_pixels; i++){
        switch(channel){
            case 0: 
                pixels[i].Set(pixels[i].r, 0, 0);
                break;
            case 1: 
                pixels[i].Set(0, pixels[i].g, 0);
                break;
            case 2: 
                pixels[i].Set(0, 0, pixels[i].b);
                break;
        }
    }
}


void Image::Quantize (int nbits)
{
    double b = pow(2, nbits);
    double f = 255.0 / (b - 1);
    for (int i = 0; i < num_pixels; i++){
        double qr = (floor(pixels[i].r * b/ 256)) * f;
        double qg = (floor(pixels[i].g * b/ 256)) * f;
        double qb = (floor(pixels[i].b * b/ 256)) * f;
        pixels[i].SetClamp(qr, qg, qb);
    }
}


void Image::RandomDither (int nbits)
{
    double b = pow(2, nbits);
    double f = 255.0 / (b - 1);
    for (int i = 0; i < num_pixels; i++){
        double qr = (floor(pixels[i].r * b/ 256)) * f;
        double qg = (floor(pixels[i].g * b/ 256)) * f;
        double qb = (floor(pixels[i].b * b/ 256)) * f;
        pixels[i].SetClamp(qr, qg, qb);
    }
}


/* Matrix for Bayer's 4x4 pattern dither. */
/* uncomment its definition if you need it */

/*
static int Bayer4[4][4] =
{
    {15, 7, 13, 5},
    {3, 11, 1, 9},
    {12, 4, 14, 6},
    {0, 8, 2, 10}
};


void Image::OrderedDither(int nbits)
{
  // For ordered dithering
  // Not required for the assignment
}

*/

/* Error-diffusion parameters for Floyd-Steinberg*/
const double
    ALPHA = 7.0 / 16.0,
    BETA  = 3.0 / 16.0,
    GAMMA = 5.0 / 16.0,
    DELTA = 1.0 / 16.0;

void Image::FloydSteinbergDither(int nbits)
{
  /* Your Work Here (Section 3.3.3) */
}

void ImageComposite(Image *bottom, Image *top, Image *result)
{
  // Extra Credit (Section 3.7).
  // This hook just takes the top image and bottom image, producing a result
  // You might want to define a series of compositing modes as OpenGL does
  // You will have to use the alpha channel here to create Mattes
  // One idea is to composite your face into a famous picture
}

void Image::Convolve(int *filter, int n, int normalization, int absval) {
  // This is my definition of an auxiliary function for image convolution 
  // with an integer filter of width n and certain normalization.
  // The absval param is to consider absolute values for edge detection.
  
  // It is helpful if you write an auxiliary convolve function.
  // But this form is just for guidance and is completely optional.
  // Your solution NEED NOT fill in this function at all
  // Or it can use an alternate form or definition
}

void Image::Blur(int n)
{
  /* Your Work Here (Section 3.4.1) */
}

void Image::Sharpen() 
{
  /* Your Work Here (Section 3.4.2) */
}

void Image::EdgeDetect(int threshold)
{
  /* Your Work Here (Section 3.4.3) */
}


Image* Image::Scale(int sizex, int sizey)
{
  /* Your Work Here (Section 3.5.1) */
  return NULL ;
}

void Image::Shift(double sx, double sy)
{
  /* Your Work Here (Section 3.5.2) */
}


/*
Image* Image::Rotate(double angle)
{
  // For rotation of the image
  // Not required in the assignment
  // But you can earn limited extra credit if you fill it in
  // (It isn't really that hard) 

    return NULL;
}
*/


void Image::Fun()
{
    /* Your Work Here (Section 3.6) */
}


Image* ImageMorph (Image* I0, Image* I1, int numLines, Line* L0, Line* L1, double t)
{
  /* Your Work Here (Section 3.7) */
  // This is extra credit.
  // You can modify the function definition. 
  // This definition takes two images I0 and I1, the number of lines for 
  // morphing, and a definition of corresponding line segments L0 and L1
  // t is a parameter ranging from 0 to 1.
  // For full credit, you must write a user interface to join corresponding 
  // lines.
  // As well as prepare movies 
  // An interactive slider to look at various morph positions would be good.
  // From Beier-Neely's SIGGRAPH 92 paper

    return NULL;
}


/**
 * Image Sample
 **/
void Image::SetSamplingMethod(int method)
{
  // Sets the filter to use for Scale and Shift
  // You need to implement point sampling, hat filter and mitchell

    assert((method >= 0) && (method < IMAGE_N_SAMPLING_METHODS));
    sampling_method = method;
}

Pixel Image::Sample (double u, double v, double sx, double sy)
{
  // To sample the image in scale and shift
  // This is an auxiliary function that it is not essential you fill in or 
  // you may define it differently.
  // u and v are the floating point coords of the points to be sampled.
  // sx and sy correspond to the scale values. 
  // In the assignment, it says implement MinifyX MinifyY MagnifyX MagnifyY
  // separately.  That may be a better way to do it.
  // This hook is primarily to get you thinking about that you have to have 
  // some equivalent of this function.

  if (sampling_method == IMAGE_SAMPLING_POINT) {
    // Your work here
  }

  else if (sampling_method == IMAGE_SAMPLING_HAT) {
    // Your work here
  }

  else if (sampling_method == IMAGE_SAMPLING_MITCHELL) {
    // Your work here
  }

  else {
    fprintf(stderr,"I don't understand what sampling method is used\n") ;
    exit(1) ;
  }

  return Pixel() ;
}

