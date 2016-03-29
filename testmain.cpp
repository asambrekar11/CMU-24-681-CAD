#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "hashset.h"


int main(void)
{
	SimpleHashSet <int> set;
	for(int i=0; i<40; ++i)
	{
		int r=rand()%100;
		set.Add(r);
		printf("%d ",r);
	}
	printf("\n");

	set.Print();
	set.Resize(17);
	set.Print();

	for(int i=0; i<50; ++i)
	{
		printf("[%d]",i);
		if(true==set.IsIncluded(i))
		{
			printf("In\n");
		}
		else
		{
			printf("Out\n");
		}
	}

	return 0;
}

