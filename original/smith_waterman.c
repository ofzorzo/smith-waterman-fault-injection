#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

double match_score=1.0;
double mismatch_score=-1.0;
double gap_score=-2.0;

void sizeof_string(FILE* fpx, FILE* fpy, long* x_num, long* y_num)
{
    *x_num=0;
    *y_num=0;
    for(*x_num=0; ; (*x_num)++){
        int ch=fgetc(fpx);
        if(ch==EOF) break;
        else if(ch=='\n' || ch==' ') (*x_num)--;
    }
    for(*y_num=0; ; (*y_num)++){
        int ch=fgetc(fpy);
        if(ch==EOF) break;
        else if(ch=='\n' || ch==' ') (*y_num)--;
    }
    if((*x_num)<=0 ||(*y_num)<=0){
        printf("file size is zero\n");
        exit(0);
    }

}

void file_read(FILE* fpx, FILE* fpy, long x_num, long y_num, char* str_x, char* str_y)
{
    long i;
    for(i=0; ; i++){
        int ch=fgetc(fpx);
        if(ch==EOF) break;
        else if(ch=='\n' || ch==' ') i--;
        else if(i<x_num) str_x[i]=ch;
    }
    for(i=0; ; i++){
        int ch=fgetc(fpy);
        if(ch==EOF) break;
        else if(ch=='\n' || ch==' ') i--;
        else if(i<y_num) str_y[i]=ch;
    }
    str_x[x_num]=0;
    str_y[y_num]=0;
}

void file_open(FILE** fpx, FILE** fpy, char* file_x_name, char* file_y_name)
{
    *fpx=fopen(file_x_name, "r");
    *fpy=fopen(file_y_name, "r");

    if(*fpx==NULL || *fpy==NULL){
        printf("file cannot open\n");
        exit(0);
    }
}

double my_maxof(double a, double b, double c, double d)
{
    double ans=a;
    if(ans<b) ans=b;
    if(ans<c) ans=c;
    if(ans<d) ans=d;
    return ans;
}

double matchMisScore(long i, long j, char* str_y, char* str_x)
{
    if (str_y[j] == str_x[i])
        return match_score;
    else
        return mismatch_score;
}

double smith_waterman(long x_num, long y_num, char* str_x, char* str_y, FILE *fo)
{
    double** matrix=(double**)calloc(x_num+1, sizeof(double*));
    matrix[0]=(double*)calloc((x_num+1)*(y_num+1), sizeof(double));
    long i;
    for(i=1; i<x_num+1; i++){
        matrix[i]=matrix[0]+(y_num+1)*i;
    }

    long j;
    for(i=0; i<x_num+1; i++){matrix[i][0]=0.0;}
    for(i=0; i<y_num+1; i++){matrix[0][i]=0.0;}

    double diff=0.0;
    double max_score=0.0;
    for(i=1; i<x_num+1; i++){
        for(j=1; j<y_num+1; j++){
            if(str_x[i-1]==str_y[j-1]){diff=match_score;}
            else{diff=mismatch_score;}
            matrix[i][j]=my_maxof(matrix[i-1][j-1]+diff, matrix[i-1][j]+gap_score, matrix[i][j-1]+gap_score,0);
            if(matrix[i][j]>max_score)
                max_score=matrix[i][j];
        }
    }

	//Print alignment matrix
    /*
    for(i=0; i<x_num+1; i++){
        for(j=0; j<y_num+1; j++){
            printf("%.2f ",matrix[i][j]);
        }
        printf("\n");
    }*/

    /*Alignment*/
    double maxValue = 0;
    long y=-1, x=-1;
    long height = x_num;
    long width = y_num;

    for(i=0; i<height+1; i++){
        for(j=0; j<width+1; j++){
            if(i==0 && j==0){
                maxValue = matrix[i][j];
                y = i;
                x = j;
            }
            if(maxValue < matrix[i][j]){
                maxValue = matrix[i][j];
                y = i;
                x = j;
            }
        }
    }
    char *alignmentA = "", *alignmentAcopy = "", *alignmentB = "", *alignmentBcopy = "";
    double score;
    double scoreDiag;
    double scoreUp;
    double scoreLeft;
    while((y>0) && (x>0) && (matrix[y][x] != 0)){
        score = matrix[y][x];
		scoreDiag = matrix[y-1][x-1];
		scoreUp = matrix[y-1][x];
		scoreLeft = matrix[y][x-1];

		if(score==(scoreDiag+matchMisScore(y-1, x-1, str_y, str_x))){
            if(strlen(alignmentA)==0){
                alignmentA = malloc(2*sizeof(char));
				snprintf(alignmentA, 2, "%c", str_y[x-1]);
            }
            else{
                alignmentAcopy = malloc((strlen(alignmentA)+1)*sizeof(char));
				snprintf(alignmentAcopy, strlen(alignmentA)+1, "%s", alignmentA);
				free(alignmentA);
				alignmentA = malloc((strlen(alignmentAcopy)+2)*sizeof(char));
				snprintf(alignmentA, strlen(alignmentAcopy)+2, "%c%s", str_y[x-1], alignmentAcopy);
                free(alignmentAcopy);
            }

            if(strlen(alignmentB)==0){
                alignmentB = malloc(2*sizeof(char));
				snprintf(alignmentB, 2, "%c", str_x[y-1]);
            }
            else{
                alignmentBcopy = malloc((strlen(alignmentB)+1)*sizeof(char));
				snprintf(alignmentBcopy, strlen(alignmentB)+1, "%s", alignmentB);
				free(alignmentB);
				alignmentB = malloc((strlen(alignmentBcopy)+2)*sizeof(char));
				snprintf(alignmentB, strlen(alignmentBcopy)+2, "%c%s", str_x[y-1], alignmentBcopy);
                free(alignmentBcopy);
            }

            x = x-1;
            y = y-1;
		}
		else if (score == (scoreLeft+gap_score)){
            if(strlen(alignmentA)==0){
                alignmentA = malloc(2*sizeof(char));
				snprintf(alignmentA, 2, "%c", str_y[x-1]);
            }
            else{
				alignmentAcopy = malloc((strlen(alignmentA)+1)*sizeof(char));
				snprintf(alignmentAcopy, strlen(alignmentA)+1, "%s", alignmentA);
				free(alignmentA);
				alignmentA = malloc((strlen(alignmentAcopy)+2)*sizeof(char));
				snprintf(alignmentA, strlen(alignmentAcopy)+2, "%c%s", str_y[x-1], alignmentAcopy);
                free(alignmentAcopy);
            }

            if(strlen(alignmentB)==0){
                alignmentB = malloc(2*sizeof(char));
				snprintf(alignmentB, 2, "-");
            }
            else{
                alignmentBcopy = malloc((strlen(alignmentB)+1)*sizeof(char));
				snprintf(alignmentBcopy, strlen(alignmentB)+1, "%s", alignmentB);
				free(alignmentB);
				alignmentB = malloc((strlen(alignmentBcopy)+2)*sizeof(char));
				snprintf(alignmentB, strlen(alignmentBcopy)+2, "-%s", alignmentBcopy);
                free(alignmentBcopy);
            }
            x = x-1;
		}
		else if(score==(scoreUp+gap_score)){
            if(strlen(alignmentA)==0){
                alignmentA = malloc(2*sizeof(char));
				snprintf(alignmentA, 2, "-");
            }
            else{
                alignmentAcopy = malloc((strlen(alignmentA)+1)*sizeof(char));
				snprintf(alignmentAcopy, strlen(alignmentA)+1, "%s", alignmentA);
				free(alignmentA);
				alignmentA = malloc((strlen(alignmentAcopy)+2)*sizeof(char));
				snprintf(alignmentA, strlen(alignmentAcopy)+2, "-%s", alignmentAcopy);
                free(alignmentAcopy);
            }

            if(strlen(alignmentB)==0){
                alignmentB = malloc(2*sizeof(char));
				snprintf(alignmentB, 2, "%c", str_x[y-1]);
            }
            else{
                alignmentBcopy = malloc((strlen(alignmentB)+1)*sizeof(char));
				snprintf(alignmentBcopy, strlen(alignmentB)+1, "%s", alignmentB);
				free(alignmentB);
				alignmentB = malloc((strlen(alignmentBcopy)+2)*sizeof(char));
				snprintf(alignmentB, strlen(alignmentBcopy)+2, "%c%s", str_x[y-1], alignmentBcopy);
                free(alignmentBcopy);
            }

            y = y-1;
		}
    }
	fprintf(fo, "Alinhamento local das sequencias:\n\nPrimeira sequencia:\n%s\n\nSegunda sequencia:\n%s", alignmentA, alignmentB);
	fprintf(fo, "\n\nScore: %f", max_score);
	free(alignmentA);
	free(alignmentB);
	free(matrix[0]);
	free(matrix);
    return max_score;
}


int main(int argc, char* argv[])
{
    clock_t t;
    t = clock();
	int opt;
    char* file_x_name="randInputX";
    char* file_y_name="randInputY";
	char* output_file_name="output";

    while((opt=getopt(argc, argv, "m:n:g:x:y:o:")) != -1){
        switch(opt){
            case 'm': //m:match score
                match_score=atof(optarg);
                break;
            case 'n': //n:mismatch score
                mismatch_score=atof(optarg);
                if(mismatch_score>0) mismatch_score*=(-1);
                break;
            case 'g': //g:gap score
                gap_score=atof(optarg);
                if(gap_score>0) gap_score*=(-1);
                break;
			case 'x':
				file_x_name = optarg;
				break;
			case 'y':
				file_y_name = optarg;
				break;
			case 'o':
				output_file_name = optarg;
				break;
        }

    }


    FILE* fpx=NULL, *fpy=NULL, *fo;

    file_open(&fpx, &fpy, file_x_name, file_y_name);
    long x_num=0, y_num=0;
    sizeof_string(fpx, fpy, &x_num, &y_num);
    fclose(fpx); fclose(fpy);

    char *str_x=(char*)calloc(x_num+1, sizeof(char));
    char *str_y=(char*)calloc(y_num+1, sizeof(char));
    file_open(&fpx, &fpy, file_x_name, file_y_name);
    file_read(fpx, fpy, x_num, y_num, str_x, str_y);
    fclose(fpx); fclose(fpy);

	fo = fopen(output_file_name, "w");
	if (fo == NULL)
	{
    	printf("Error opening file!\n");
    	exit(1);
	}

    smith_waterman(x_num, y_num, str_x, str_y, fo);

	fclose(fo);
	free(str_x);
	free(str_y);

	t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
	printf("Alignment took %f seconds to execute \n", time_taken); 
    return(0);
}