#include "ImageList.h"
#include "Movie.h"
#include "DIPs.h"

/* Delete an Image Entry from Movie */
void DeleteImageEntry(IENTRY *e);

/* Remove First Image Entry */
void RemoveFirstEntry(ILIST *l);

/* Remove Last Image Entry */
void RemoveLastEntry(ILIST *l);


/* allocate a new image list */
ILIST *NewImageList(void)
{

	ILIST *new = (ILIST*)malloc(sizeof(ILIST));
	
	new->First = NULL;
	new->Last = NULL;
	new->Length = 0;
	
	return new;

} /*End of NewImageList*/

/* delete a image list (and all entries) */
void DeleteImageList(ILIST *l)
{

	assert(l);
	
	IENTRY *s;
	IENTRY *e;
	
	s = l->First;
	while (s) /* if no List, First = 0 */
	{
		e = s->Next;
		DeleteImage(s->Image);
		s->Image = NULL;
		free(s);
		s = e;
	}
	free(l);
} /* End of DeleteImageList */

/* insert a frame into a list at the end*/
void AppendImage(ILIST *l, IMAGE *image)
{

	assert(l);
	assert(image);

	IENTRY *new;

	new = (IENTRY *)malloc(sizeof(IENTRY));
	new->Prev = NULL;
	new->Next = NULL;
	new->List = l;
	new->Image = image;
	
	if(!l->First){ /* if length = 0 */
		l->First = l->Last = new;
	}
	else{
		l->Last->Next = new;
		new->Prev = l->Last;
		l->Last = new;
	}
	l->Length ++;

} /* End of AppendImage */


/* reverse an image list */
void ReverseImageList(ILIST *l)
{
	assert(l);
	int x;
	
	IENTRY *e;
	IENTRY *temp;
	e = NULL;
	
	temp = l->First;
	e = l->First;
	l->First = l->Last;
	l->Last = temp;

	for (x = 0; x < l->Length; x++)
	{
		temp = e->Next;
		e->Next = e->Prev;
		e->Prev = temp;
		e = temp;
	}
} /* End of ReverseImageList */

/*Crop frames from the list starting from SFrame to EFrame*/
void CropImageList(ILIST *l, unsigned int SFrame, unsigned int EFrame)
{
	assert(l);
	unsigned int x = 0;
	unsigned int subEFrame;
	
	if (SFrame < 1 || EFrame > l->Length)
		{
			printf("parameters for -cut is incorrect.\n");
			return;
		}
	
	subEFrame = l->Length - EFrame;
	
	if ( EFrame < l->Length)
		{
			for (x = 0; x < subEFrame; x++)
				{
					RemoveLastEntry(l);
					
				}
		}
	
	if ( SFrame > 1)
		{
			for (x = 0; x < SFrame; x++)
				{
					RemoveFirstEntry(l);
					
				}
		}
} /*End of CropImageList */

/*Resize the images in the list*/
void ResizeImageList(ILIST *l, unsigned int percentage)
{
	unsigned int x;
	assert(l);
	assert(percentage);
	
	IENTRY *e;
	IMAGE *temp;
	IMAGE *s;

	e = l->First;
	
	if(percentage <= 0 || percentage > 100){
		printf("Resizing percentage %d%% is out of the range, Sorry! \n", percentage);
		return;
	}

	for(x = 0; x < l->Length; x++)
		{
			s = e->Image;
			
			YUV2RGBImage(s); /* converts color space */
			temp = Resize(percentage, s);
			RGB2YUVImage(s);/* converts color space */
			
			RGB2YUVImage(temp);

			DeleteImage(s);
			e->Image = temp;
			e = e->Next;

		}
} /* End of Resize Image List */

/*Fast forward the video*/
unsigned int FastImageList(ILIST *l, unsigned int ff_factor)
{
	unsigned int counter = 0, tracker;
	
	tracker = l->Length;
	
	assert(l);
	assert(ff_factor);
	
	IENTRY *e;
	IENTRY *temp = NULL;
	
	
	if (l->First){ /*if no list, first = 0 */
		e = l->First;
	}
	else{
		return l->Length;
	}
	
	if (ff_factor == 1){
		printf("-fast parameter of 1 is normal speed!\n");
		return l->Length;
	}
	
	while(e->Next) /* ends when next is NULL*/
	{
		counter = counter % ff_factor; /* resets counter */
		if (counter == 0)
			{
				temp = e->Next;
				e = temp;
				l->Last = e;
			}
		else
			{
				temp = e->Next;
				e->Prev->Next = e->Next;
				e->Next->Prev = e->Prev;
				DeleteImageEntry(e);
				free(e);
				e = temp;
				l->Length--;
				l->Last = e;
			}
		counter++;
	}
	
	while (l->Length > tracker/ff_factor) /* if length of movie is not what its suppose to be */
		{
			RemoveLastEntry(l);
		}
	
	return l->Length;
} /* End of FastImageList*/

/* Delete Image Entry */
void DeleteImageEntry(IENTRY *e)
{
	assert(e);
	
	DeleteImage(e->Image);
	e->Image = NULL;
} /* End of DeleteImageEntry */

/* Remove First Image Entry */
void RemoveFirstEntry(ILIST *l)
{
	IENTRY *e = NULL;
	assert(l);
	if (l->First)
		{
			e = l->First;
			l->First = e->Next;
			if (l->First)
				{
					l->First->Prev = NULL;
				}
			else
				{
					assert(l->Last);
					l->Last = NULL;
				}
			l->Length--;
			DeleteImageEntry(e);
			free(e);
		}
	else
		{
			return;
		}
} /* end of RemoveFirstEntry */

/* Remove Last Image Entry */
void RemoveLastEntry(ILIST *l)
{
	IENTRY *e = NULL;
	assert(l);
	if (l->Last)
		{
			e = l->Last;
			l->Last = e->Prev;
			if (l->Last)
				{
					l->Last->Next = NULL;
				}
			else	
				{
					assert(l->First);
					l->First = NULL;
				}
			l->Length--;
			DeleteImageEntry(e);
			free(e);
		}
	else
		{
			return;
		}
} /* end of RemoveLastEntry */

/* EOF ImageList.c */