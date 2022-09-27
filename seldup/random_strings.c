#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_CHARS 15000

int main(){
	int i = 0;
	int j = -1;
	char str_1[MAX_CHARS+1];
	char str_2[MAX_CHARS+1];
	srand(time(NULL));
	for(i=0; i<MAX_CHARS; i++)
	{
		str_1[i] = (rand() % (90 - 65 + 1)) + 65;
		str_2[i] = str_1[i];
	}
	str_1[MAX_CHARS] = '\0';
	str_2[MAX_CHARS] = '\0';

	//cut second string
	float cut = (rand() % (100 - 65 + 1)) + 65;
	cut = (cut*MAX_CHARS)/100.0f;
	cut = ceil(cut);
	int cutting_point = cut;
	str_2[cutting_point] = '\0';

	//change second string a little
	int percentage_to_change = (rand() % (75 - 20 + 1)) + 20; // muda a segunda string de 20 a 75%
	for(i=0; i<cutting_point-1; i++){
		int change_current = (rand() % (100 - 0 + 1)) + 0;
		if(change_current < percentage_to_change){
			str_2[i] = str_2[i+1];
		}
	}

	FILE *fx=NULL, *fy=NULL;
	int x_or_y = (rand() % (1 - 0 + 1)) + 0;
	if(x_or_y == 1){
		fx = fopen("randInputX", "w");
		if (fx == NULL)
		{
			printf("Error opening file!\n");
			exit(1);
		}
		fy = fopen("randInputY", "w");
		if (fy == NULL)
		{
			printf("Error opening file!\n");
			exit(1);
		}
		fprintf(fx, "%s", str_1);
		fprintf(fy, "%s", str_2);
	}
	else{
		fx = fopen("randInputX", "w");
		if (fx == NULL)
		{
			printf("Error opening file!\n");
			exit(1);
		}
		fy = fopen("randInputY", "w");
		if (fy == NULL)
		{
			printf("Error opening file!\n");
			exit(1);
		}
		fprintf(fy, "%s", str_1);
		fprintf(fx, "%s", str_2);
	}
	fclose(fx);
	fclose(fy);
	return 0;
}
