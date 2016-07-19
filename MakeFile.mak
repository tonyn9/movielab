
#design names
DESIGN = MovieLab


#libs
LIBS = libfilter.a libother.a


CC		= gcc
CFLAGS	= -ansi -Wall -c
LFLAGS	= -Wall
AR		= ar rc
RANLIB	= ranlib


MOVIES = out.yuv out1.yuv out2.yuv mandelbrot.yuv

all: MovieLab

#target to generate MovieLab.o
MovieLab.o: MovieLab.c
		$(CC) $(CFLAGS) PhotoLab.c -o PhotoLab.out
		
#target to generate Image.o
Image.o: Image.h Image.c
		$(CC) $(CFLAGS) Image.c -o Image.o
		
#target to generate ImageList.o
ImageList.o: ImageList.h ImageList.c
		$(CC) $(CFLAGS) ImageList.c -o ImageList.o
		
#target to generate Movie.o
Movie.o: Movie.h Movie.c
		$(CC) $(CFLAGS) Movie.c -o Movie.o
		
#target to generate DIPs.o
DIPs.o: DIPs.h DIPs.c
		$(CC) $(CFLAGS) DIPs.c -o DIPs.o
		
#libs? 

#target to generate libfilter.a
libfilter.a: DIPs.o Image.o
		$(AR) libfilter.a DIPs.o Image.o
		$(RANLIB) libfilter.a

#target to generate libother.a
libother.a: ImageList.o Movie.o
		$(AR) libother.a ImageList.o Movie.o
		$(RUNLIB) libother.a
		
#target to generate MovieLab
MovieLab: MovieLab.o libother.a libfilter.a
		$(CC) $(LFLAGS) MovieLab.o -lfilter -lother -L. -o MovieLab
		

#target to clean directory
clean:
		rm -f *.o $(DESIGN) $(MOVIES) $(LIBS)