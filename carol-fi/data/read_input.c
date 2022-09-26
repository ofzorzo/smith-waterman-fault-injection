#include <stdlib.h>
#include <stdio.h>

#include "malloc2d.h"
#include "input_synth.h"


int main() {
  float **input_matrix;
  float **input_kernels;

  printf("Reading input matrix\n");
  input_matrix = read_persisted_matrix("data/new_matrix.bin");
  print_matrix(input_matrix, 5, 5);

  printf("Reading input kernels\n");
  input_kernels = read_persisted_kernels("data/new_kernels.bin");
  print_kernels(input_kernels, 5);

  printf("Freeing\n");
  free_2d(S, input_matrix);
  free_2d(M, input_kernels);
}
