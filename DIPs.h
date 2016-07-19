

#ifndef DIPS_H
#define DIPS_H

#include <stdlib.h>
#include <assert.h>

#include "Movie.h"


/*** DIP Function Declarations ***/

/* change color image to black & white */
void BlackNWhite(IMAGE *image);

/* flip image vertically */
void VFlip(IMAGE *image);

/* mirror image horizontally */
void HMirror(IMAGE *image);

/* Edge Detection */
void Edge(IMAGE *image);

/* Posterize */
void Posterize(IMAGE *image);

/* Resize */
IMAGE *Resize(unsigned int percentage, IMAGE *image);

/* Mandelbrot						*/
IMAGE *Mandelbrot(unsigned int W, unsigned int H, unsigned int max_iteration, long double zoom);

#endif /* DIPS_H */


/* EOF DIPs.h */