/*********************************************************************/
/* MovieLab.c: homework assignment #5, for EECS 22,	Fall 2011			 */
/*	MovieLab by Tony Nguyen												*/
/*																																	 */
/* History:																													*/
/* 10/30/11 Weiwei Chen	updated for EECS22 assignment5 FAll2011		 */
/* 10/30/11 Weiwei Chen	: initial template version									 */
/*********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ImageList.h"
#include "Movie.h"
#include "Image.h"
#include "DIPs.h"


/*save the movie frames to the output file */
int SaveMovie(const char *fname, MOVIE *movie);

/*read one frame from the movie */
IMAGE* ReadOneFrame(const char* fname, int nFrame, unsigned int W, unsigned H);

/*read the movie frames from the input file */
int ReadMovie(const char *fname, int nFrame, unsigned int W, unsigned H, MOVIE *movie);

/* Print the command line parameter usage of the program*/
void PrintUsage();

/* type define the function pointer to the DIP function	*/
typedef void MOP_F(IMAGE *image);

/* the function for perform DIP operations on the movie*/
void Movie_DIP_Operation(MOVIE *movie, MOP_F *MovieOP);

/*Fill the Mandelbrot images as the frames to a movie in MovieLab.c */
int MandelbrotMovie(int nFrame, /*number of frames in the movie*/
unsigned int W, /*the width of the movie*/
unsigned H, /*the height of the movie*/
MOVIE *movie /* the pointer to the output movie*/
);


int main(int argc, char *argv[])
{
	int x = 0;
	int bw = 0, vf = 0, hm = 0, e = 0, p = 0, m = 0;
	int c = 0, rv = 0, rs = 0, ff = 0;
	char *fin = NULL, *fout = NULL;
	int W = 0, H = 0, numFrames = 0;
	int sFrame = 0, eFrame = 0, res = 0, fFrame = 0;
	MOVIE *movie;

 /*entering while loop to check options entered*/
	while(x < argc)
	{
		/*specify input video*/
		if(0 == strcmp(&argv[x][0], "-i")) 
		{
			if(x < argc - 1)
			{
				fin = (char *)malloc(sizeof(char) * (strlen(&argv[x + 1][0]) + strlen(".yuv") + 1));
				strcpy(fin, argv[x + 1]);
				strcat( fin, ".yuv");
			}/*fi*/
			else
			{
				printf("Missing argument for input name!");
				free(fin);
				free(fout);
				return 5;
			}/*esle*/
			x += 2;
			continue;
		}/*fi*/

		/*specify output video*/
		if(0 == strcmp(&argv[x][0], "-o")) 
		{
			if(x < argc - 1)
			{
				fout = (char *)malloc(sizeof(char) * (strlen(&argv[x + 1][0]) + strlen(".yuv") + 1));
				strcpy(fout, argv[x + 1]);
				strcat( fout, ".yuv");
			}/*fi*/
			else
			{
				printf("Missing argument for output name!");
				free(fin);
				free(fout);
				return 5;
			}/*esle*/
			x += 2;
			continue;
		}/*fi*/
		
		/*set resolution of input stream */
		if(0 == strcmp(&argv[x][0], "-s"))
		{
			if (x < argc - 1)
			{
				if(sscanf(argv[x + 1], "%dx%d", &W, &H) == 2)
				{
					assert(W);
					assert(H);
				}
				
				else
				{
					printf("not.\nResolution information provided by -s parameter is incorrect.\n");
					PrintUsage();
					free(fin);
					free(fout);
					return 5;
				}
			}
			else
			{
				printf("t.\nResolution information provided by -s parameter is incorrect.\n");
				PrintUsage();
				free(fin);
				free(fout);
				return 5;
			}
			x += 2;
			continue;
		} /*fi */
		
		if(0 == strcmp(&argv[x][0], "-f"))
		{
			if (x < argc - 1)
			{
				if(sscanf(argv[x + 1], "%d", &numFrames) == 1)
				{
					assert(numFrames);
				}
				else
				{
					printf("Number of frames information provided by -f parameter is incorrect.\n");
					PrintUsage();
					free(fin);
					free(fout);
					return 5;
				}
			}
			else
			{
				printf("Number of frames information provided by -f parameter is incorrect.\n");
				PrintUsage();
				free(fin);
				free(fout);
				return 5;
			}
			x += 2;
			continue;
		} /* fi */
		
		if ( 0 == strcmp(&argv[x][0], "-m"))
		{
			m = 1;
			x++;
			continue;
		}
		
		if( 0 == strcmp(&argv[x][0], "-bw"))
		{
			bw = 1;
			x++;
			continue;
		} /* fi */
		
		if( 0 == strcmp(&argv[x][0], "-vflip"))
		{
			vf = 1;
			x++;
			continue;
		} /* fi */
		
		if( 0 == strcmp(&argv[x][0], "-hmirror"))
		{
			hm = 1;
			x++;
			continue;
		} /* fi */

		if( 0 == strcmp(&argv[x][0], "-edge"))
		{
			e = 1;
			x++;
			continue;
		} /* fi */

		if( 0 == strcmp(&argv[x][0], "-poster"))
		{
			p = 1;
			x++;
			continue;
		} /* fi */
		
		if ( 0 == strcmp(&argv[x][0], "-rvs"))
		{
			rv = 1;
			x++;
			continue;
		}

		if( 0 == strcmp(&argv[x][0], "-cut"))
		{
			if (x < argc -1)
			{
				if(sscanf(argv[x + 1], "%d-%d", &sFrame, &eFrame) == 2)
				{
					assert(sFrame);
					assert(eFrame);
				}
				else
				{
					printf("Frames information given by -cut parameter is incorrect.\n");
					PrintUsage();
					free(fin);
					free(fout);
					return 5;
				}
			}
			else
			{
				printf("Frames information given by -cut parameter is incorrect. \n");
				PrintUsage();
				free(fin);
				free(fout);
				return 5;
			}
			c = 1;
			x += 2;
			continue;
		} /* fi */
		
		if( 0 == strcmp(&argv[x][0], "-resize"))
		{
			if (x < argc - 1)
			{
				if (sscanf(argv[x + 1], "%d", &res) == 1)
				{
					assert(res);
				}
				else
				{
					printf("Resolution information given by -resize parameter is incorrect.\n");
					PrintUsage();
					free(fin);
					free(fout);
					return 5;
				}
			}
			else
			{
				printf("Resolution information given by -resize parameter is incorrect.\n");
				PrintUsage();
				free(fin);
				free(fout);
				return 5;
			}
			rs = 1;
			x += 2;
			continue;
		} /* fi */
		
		if ( 0 == strcmp(&argv[x][0], "-fast"))
		{
			if (x < argc - 1)
			{
				if (sscanf(argv[x + 1], "%d", &fFrame) == 1)
				{
					assert(fFrame);
				}
				else
				{
					printf("Frames information given by -fast parameter is incorrect.\n");
					PrintUsage();
					free(fin);
					free(fout);
					return 5;
				}
			}
			else
			{
				printf("Frames information given by -fast parameter is incorrect.\n");
				PrintUsage();
				free(fin);
				free(fout);
				return 5;
			}
			ff = 1;
			x += 2;
			continue;
		} /* fi */
		
		/*help option*/
		if(0 == strcmp(&argv[x][0], "-h")) 
		{ 
			PrintUsage();
			free(fin);
			free(fout);
			return 0;
		}/*fi*/
		x++;
	}/*elihw*/

	if(!fin && !m){
		printf("Missing argument for input name!\n");
		PrintUsage();
		free(fin);
		free(fout);
		return 5;
	}

	if(!fout){
		printf("Missing argument for output name!\n");
		PrintUsage();
		free(fin);
		free(fout);
		return 5;
	}
	
	if(W == 0 || H == 0)
	{
		printf("Test.\nNo resolution information provided by -s parameter.\n");
		PrintUsage();
		free(fin);
		free(fout);
		return 5;
	}
	
	if(numFrames == 0)
	{
		printf("Test.\nNumber of frames information provided by -f parameter is incorrect.\n");
		PrintUsage();
		free(fin);
		free(fout);
		return 5;
	}
	
	movie = CreateMovie(numFrames, W, H);
	if (m == 1)
	{
		MandelbrotMovie(numFrames, W, H, movie);
		printf("Operation Mandelbrot is done!\n");
	}
	else{
		
		ReadMovie(fin, numFrames, W, H, movie);
		if(bw == 1)
		{
			Movie_DIP_Operation(movie, BlackNWhite);
			printf("Operation BlackNWhite is done!\n");
		}

		if(vf == 1)
		{
			Movie_DIP_Operation(movie, VFlip);
			printf("Operation VFlip is done!\n");
		}	
		
		if(hm == 1)
		{
			Movie_DIP_Operation(movie, HMirror);
			printf("Operation HMirror is done!\n");
		}
		
		if(e == 1)
		{
			Movie_DIP_Operation(movie, Edge);
			printf("Operation Edge is done!\n");
		}
		
		if(p == 1)
		{
			Movie_DIP_Operation(movie, Posterize);
			printf("Operation Posterize is done!\n");
		}	
		
		if(rv == 1)
		{
			ReverseImageList(movie->Frames);
			printf("Operation VFlip is done!\n");
		}
		
		
		if(rs == 1)
		{
			ResizeImageList(movie->Frames,res);
			printf("Operation Resize is done! New Width/Height = %dx%d\n", (W*res/100), (H*res/100));
			movie->Width = (W*res/100);
			movie->Height = (H*res/100);
		}
		
		if(c == 1)
		{
			CropImageList(movie->Frames, sFrame, eFrame);
			printf("Operation Frame Cropping is done!\n");
			movie->NumFrames = movie->Frames->Length;
		}	
		
		if(ff == 1)
		{
			
			printf("Operation Fast Forward is done! Number of frames = %u.\n", FastImageList(movie->Frames, fFrame));
			movie->NumFrames = movie->Frames->Length;
		}
	}
	
	/*printf("%d", movie->Frames->Length);*/
	SaveMovie(fout, movie);
	
	DeleteMovie(movie);
	movie = NULL;
	
	
	free(fin);
	free(fout);
	fin = NULL;
	fout = NULL;

	return 0;
}

/* the function for perform DIP operations on the movie*/
void Movie_DIP_Operation(MOVIE *movie, MOP_F *MovieOP)
{
	
	IENTRY *curr;
	
	curr = movie->Frames->First;
	/*printf("%d\n", movie->Frames->Length);*/
	while(curr)
	{
		YUV2RGBImage(curr->Image);
		MovieOP(curr->Image);
		RGB2YUVImage(curr->Image);
		curr = curr->Next;
	}
	
}


void PrintUsage()
{
	printf("\nFormat on command line is:\n"
	"MovieLab [option]\n"
	"-i [file_name]    to provide the input file name\n"
	"-o [file_name]    to provide the	output file name\n"
	"-f [no_frames]    to determine how many frames desired in the input stream\n"
	"-s [WidthxHeight] to set resolution of the input stream (widthxheight)\n"
	"-m                to generate the movie with Mandelbrot sequences\n"
	"-bw               to activate the conversion to black and white\n"
	"-vflip            to activate vertical flip\n"
	"-hmirror          to activate horizontal flip\n"
	"-edge             to activate edge filter\n"
	"-poster           to activate posterize filter\n"
	"-cut [Start-End]  to crop the frame from the video from frame Start to frame End\n"  
	"-resize [factor]  to resize the video with the provided factor [1-100]\n"
	"-fast   [factor]  to fast forward the video with the provided factor [1+]\n"
	"-rvs              to reverse the frame order of the input stream\n"
	"-h                to show this usage information\n"
	);
}

IMAGE* ReadOneFrame(const char *fname, int nFrame, unsigned int W, unsigned H)
{
        /*defining local variables*/
        FILE *file;

        unsigned int x, y;
        unsigned char ch;
        IMAGE* image ;

        /*checking error*/
        assert(fname);
        assert(nFrame >= 0);

        image = CreateImage(W, H) ;
        assert(image) ;

        /*opening file stream*/
        file = fopen(fname, "r");
        assert(file) ;

        /*find desired frame*/
        fseek(file, 1.5 * nFrame * W * H, SEEK_SET);

        for(y = 0; y < H; y ++){
                for(x = 0; x < W; x ++){
                        ch = fgetc(file);
                        SetPixelY(image, x, y, ch);
                }/*rof*/
        }

        for(y = 0; y < H ; y += 2){
                for(x = 0; x < W ; x += 2){
                        ch = fgetc(file);
                        SetPixelU(image, x, y, ch);
                        SetPixelU(image, x + 1, y, ch);
                        SetPixelU(image, x, y + 1, ch);
                        SetPixelU(image, x + 1, y + 1, ch);
                }
        }

        for(y = 0; y < H ; y += 2){
                for(x = 0; x < W ; x += 2){
                        ch = fgetc(file);
                        SetPixelV(image, x, y, ch);
                        SetPixelV(image, x + 1, y, ch);
                        SetPixelV(image, x, y + 1, ch);
                        SetPixelV(image, x + 1, y + 1, ch);
                }
        }

        /*checking for error*/

        assert(ferror(file) == 0) ;

        /*closing stream and terminating*/
        fclose(file);
        file = NULL;
        return image;
}


/*save the movie frames to the output file */
int SaveMovie(const char *fname, MOVIE *movie)
{
	int i ; 
	int x, y;
	FILE *file;
	IENTRY *currFrame;

	 /*opening file stream*/
	 if(!(file = fopen(fname, "w"))){
			return 1;
	 }
	 
	 i = 0 ; 
	 printf("start savemovie, total frame %d\n", movie->NumFrames) ; 
	 currFrame = movie->Frames->First;
	 while(currFrame){
		for(y = 0; y < movie->Height; y ++){ 
			for(x = 0; x < movie->Width; x ++){
				fputc(GetPixelY(currFrame->Image, x, y), file);
			}
		}
		for(y = 0; y < movie->Height; y += 2){ 
			for(x = 0; x < movie->Width; x += 2){
				fputc(GetPixelU(currFrame->Image, x, y), file);
			}
		}
		for(y = 0; y < movie->Height; y += 2){ 
			for(x = 0; x < movie->Width; x += 2){
				fputc(GetPixelV(currFrame->Image, x, y), file);
			}
		}
		currFrame = currFrame->Next;	 
		i++ ; 
	 }

	 fclose(file);
	 file = NULL;

	 printf("The movie file %s has been written successfully! \n", fname);
	 printf("%d frames are written to the file %s in total \n", i, fname); 
	 return 0;
}

/*read the movie frames from the input file */
int ReadMovie(const char *fname, int nFrame, unsigned int W, unsigned H, MOVIE *movie)
{
	/* defining local variables */
	int x;
	
	/* error checking */
	assert(fname);
	assert(movie);
	
	
	for (x = 0; x < nFrame; x++)
	{	

		AppendImage(movie->Frames, ReadOneFrame(fname, x, W, H));
	}
	

	printf("The movie file %s has been read successfully! \n", fname);
	return 0;
}

/*Fill the Mandelbrot images as the frames to a movie in MovieLab.c */
int MandelbrotMovie(int nFrame, /*number of frames in the movie*/
unsigned int W, /*the width of the movie*/
unsigned H, /*the height of the movie*/
MOVIE *movie /* the pointer to the output movie*/
)
{
	/*IMAGE* image;
	image = CreateImage(W, H);
	int x, y;
	
	long double n = 1.17, result = 1.0;
	for (x = 1; x < nFrame +1; x++)
		{	
			result = 1.0;
			y = x;
			while ( y--)
			{
				result *= n; 
			}
			
			printf("%lf\n", result);
			printf("Creating Mandelbrot frame #%d\n", x);
			image = Mandelbrot(W, H, 2000, result);
			AppendImage(movie->Frames, image);
			
		}*/
		
	printf("Sorry, this programmer sucks a lot so no mandelbrot function~~ \n");
	return 0;
}
