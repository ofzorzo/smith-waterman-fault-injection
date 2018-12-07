#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

double match_score=1.0;
double match_score2=1.0;
double mismatch_score=-1.0;
double mismatch_score2=-1.0;
double gap_score=-2.0;
double gap_score2=-2.0;


char* log="detected.log";
char* log2="detected.log";

void sdc_exit(){
    if(strcmp(log, log2) != 0){
        FILE* fe;
        fe = fopen(log, "a+");
        if (fe == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }
        fprintf(fe, "SDC ocorreu.\n");
        fclose(fe);
        
        FILE* fe2;
        fe2 = fopen(log2, "a+");
        if (fe2 == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }
        fprintf(fe2, "SDC ocorreu.\n");
        fclose(fe2);
    }
    else{
        FILE* fe;
        fe = fopen(log, "a+");
        if (fe == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }
        fprintf(fe, "SDC ocorreu.\n");
        fclose(fe);
    }
    exit(0);
}

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

void check_index(int i, int j){
    if(i != j){
        sdc_exit();
    }
}

char *smith_waterman(long x_num, long x_num2, long y_num, long y_num2, char* str_x, char* str_y)
{
    if(x_num != x_num2){        
        sdc_exit();
    }

	if(y_num != y_num2){        
        sdc_exit();
    }

    double** matrix=(double**)calloc(x_num+1, sizeof(double*));
    matrix[0]=(double*)calloc((x_num+1)*(y_num+1), sizeof(double));
    long i, i2;
    i2 = 1;
    for(i=1; i<x_num+1; i++){
        check_index(i, i2);
        matrix[i]=matrix[0]+(y_num+1)*i;
        i2++;
    }
    check_index(i, i2);

    long j, j2;
	
    i2 = 0;
    for(i=0; i<x_num+1; i++){
        check_index(i, i2);
        matrix[i][0]=0.0;
        i2++;
    }
    check_index(i, i2);
    i2 = 0;
    for(i=0; i<y_num+1; i++){
        check_index(i, i2);
        matrix[0][i]=0.0;
        i2++;
    }
	check_index(i, i2);

    double diff=0.0;
    double max_score=0.0;
    i2 = 1;
    for(i=1; i<x_num+1; i++){
        check_index(i, i2);
        j2 = 1;
        for(j=1; j<y_num+1; j++){
            check_index(j, j2);
            if(str_x[i-1]==str_y[j-1]){diff=match_score;}
            else{diff=mismatch_score;}
            matrix[i][j]=my_maxof(matrix[i-1][j-1]+diff, matrix[i-1][j]+gap_score, matrix[i][j-1]+gap_score,0);
            if(matrix[i][j]>max_score)
                max_score=matrix[i][j];
            j2++;
        }
        i2++;
    }
    check_index(j, j2);
    check_index(i, i2);

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

    i2 = 0;
    for(i=0; i<height+1; i++){
        check_index(i, i2);
        j2 = 0;
        for(j=0; j<width+1; j++){
            check_index(j, j2);
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
            j2++
        }
        i2++;
    }
    check_index(j, j2);
    check_index(i, i2);

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
	int stringSize = strlen(alignmentA) + strlen(alignmentB);
	stringSize = stringSize + 76; //nro de caracteres da frase
	stringSize = stringSize + 1; //considera caractere de fim de string: \0
	char *answer = malloc(stringSize*sizeof(char));
	int error = snprintf(answer, stringSize, "Alinhamento local das sequencias:\n\nPrimeira sequencia:\n%s\n\nSegunda sequencia:\n%s", alignmentA, alignmentB);

	free(alignmentA);
	free(alignmentB);
	free(matrix[0]);
	free(matrix);
    return answer;
}

char *execute(char *file_x_name, char *file_y_name){
	FILE* fpx=NULL, *fpy=NULL;

    file_open(&fpx, &fpy, file_x_name, file_y_name);
    long x_num=0, y_num=0;
    sizeof_string(fpx, fpy, &x_num, &y_num);
    fclose(fpx); fclose(fpy);

    char *str_x=(char*)calloc(x_num+1, sizeof(char));
    char *str_y=(char*)calloc(y_num+1, sizeof(char));
    file_open(&fpx, &fpy, file_x_name, file_y_name);
    file_read(fpx, fpy, x_num, y_num, str_x, str_y);
    fclose(fpx); fclose(fpy);

	char *answer = smith_waterman(x_num, x_num, y_num, y_num, str_x, str_y);
	free(str_x);
	free(str_y);
	return answer;
}


int main(int argc, char* argv[])
{
	int opt;
    char* file_x_name="randInputX";
    char* file_y_name="randInputY";
	char* output_file_name="output";

    while((opt=getopt(argc, argv, "m:n:g:x:y:o:z:")) != -1){
        switch(opt){
            case 'm': //m:match score
                match_score=atof(optarg);
				match_score2=atof(optarg);
                break;
            case 'n': //n:mismatch score
                mismatch_score=atof(optarg);
				mismatch_score2=atof(optarg);
                if(mismatch_score>0) mismatch_score*=(-1);
                break;
            case 'g': //g:gap score
                gap_score=atof(optarg);
				gap_score2=atof(optarg);
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
			case 'z':
				log = optarg;
				log2 = optarg;
				break;
        }

    }
    
    if( (match_score != match_score2) || (gap_score != gap_score2) || (mismatch_score != mismatch_score2) )
    {
        sdc_exit()
    }
    else{
        
        char *answer1 = execute(file_x_name, file_y_name);
        
        //writes final answer to disk (if SDC detected, the final answer can be masked if the SDC occurred in answer2, or a SDC if in answer1)
    	FILE* fo;
    	fo = fopen(output_file_name, "w");
    	if (fo == NULL)
    	{
        		printf("Error opening file!\n");
        		exit(1);
    	}
    	fprintf(fo, "%s", answer1);
    	fclose(fo);
    }

    return(0);
}
