
#include "Movie.h"

/* Provides clipping for conversion of color space */
unsigned char clip(int x)
{
	if (x > 255)
	{
		return 255;
	}
	else if (x < 0)
	{
		return 0;
	}
	else
	{
		return x;
	}
}

/* allocate the memory space for the movie */
/* and the memory space for the frame list. */
/* return the pointer to the movie */
MOVIE *CreateMovie(unsigned int nFrames, unsigned int W, unsigned int H)
{
	assert(nFrames);
	assert(W);
	assert(H);

	MOVIE *movie;
	if ( !(movie = (MOVIE *)malloc(sizeof(MOVIE))))
	{
		return NULL;
	}
	
	movie->NumFrames = nFrames;
	movie->Width = W;
	movie->Height = H;
	movie->Frames = NewImageList();

	return movie;
}

/*release the memory space for the frames and the frame list. */
/*release the memory space for the movie. */
void DeleteMovie(MOVIE *movie)
{
	assert(movie);

	DeleteImageList(movie->Frames);
	movie->Frames = NULL;
	
	free(movie);

}

/* convert a YUV image into a RGB image */
void YUV2RGBImage(IMAGE *Image)
{
	assert(Image);
	
	int x, y;
	int C, D, E;
	
	for(x = 0; x < Image->Width; x++)
	{
		for(y = 0; y < Image->Height; y++)
		{
			C = GetPixelY(Image, x, y) - 16;
			D = GetPixelU(Image, x, y) - 128;
			E = GetPixelV(Image, x, y) - 128;
			
			SetPixelR(Image, x, y, clip((298*C+409*E+128)>>8));
			SetPixelG(Image, x, y, clip((298*C-100*D-208*E+128)>>8));
			SetPixelB(Image, x, y, clip((298*C+516*D+128)>>8));
		
		}
	
	}

}

/* convert a RGB image into a YUV image */
void RGB2YUVImage(IMAGE *Image)
{
	assert(Image);

	int x, y;
	int R, G, B;
	
	for (x = 0; x < Image->Width; x++)
	{
		for (y = 0; y < Image->Height; y++)
		{
			R = GetPixelR(Image, x, y);
			G = GetPixelG(Image, x, y);
			B = GetPixelB(Image, x, y);
			
			SetPixelY(Image, x, y, clip(((66 *R+129 *G+25 *B+128) >> 8) +16));
			SetPixelU(Image, x, y, clip(((-38 *R-74 *G+112 *B+128) >> 8) +128));
			SetPixelV(Image, x, y, clip(((112 *R-94 *G-18 *B+128) >> 8) +128));

		}
	}

}



/* EOF Movie.c*/