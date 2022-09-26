#include <stdlib.h>
#include "malloc2d.h"

float** malloc_2d(int n2, int n1, float initial_value) {
  float **matrix = (float**) malloc(n2*sizeof(float*));

  for(int i=0; i<n2; i++){
    matrix[i] = (float*) malloc(n1*sizeof(float));
    for(int j=0; j<n1; j++) {
      matrix[i][j] = initial_value;
    }
  }

  return matrix;
}

void free_2d(int n2, float** matrix) {
  for(int i=0; i<n2; i++){
    free(matrix[i]);
  }
  free(matrix);
}
