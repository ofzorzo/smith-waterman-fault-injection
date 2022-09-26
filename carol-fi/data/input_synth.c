#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <limits.h>

#include "malloc2d.h"
#include "input_synth.h"

// VISUALIZATION
void print_matrix(float **matrix, int limit_n2, int limit_n1){
  for(int y=0; y<limit_n2; y++) {
    for(int x=0; x<limit_n1; x++) {
      printf("| %1.4f ", matrix[y][x]);
    }
    printf("|\n");
  }
  printf("\n");
}

void print_kernels(float **kernels, int limit){
  float **kernel_buffer = malloc_2d(K, K, 0.0);

  for(int m=0; m<limit; m++) {
    kernel_matrix_from_line(kernels[m], kernel_buffer);
    print_matrix(kernel_buffer, K, K);
  }

  free_2d(K, kernel_buffer);
}

// WRITE
float random_float() {
  return -1. + 2.*rand() / (float) INT_MAX;
}

void persist_input_matrix() {
  float **im_in = malloc_2d(S, S, 0.0);

  FILE *output_ptr;
  int buffer_i;
  float buffer_f;

  for(int y=0; y<S; y++){
    for(int x=0; x<S; x++) {
      im_in[y][x] = random_float();
    }
  }

  print_matrix(im_in, 5, 5);

  /* file output */
  output_ptr = fopen("new_matrix.bin", "wb");
  {
    // header
    buffer_i = S;
    fwrite(&buffer_i, sizeof(int), 1, output_ptr);

    // data
    for(int y=0; y<S; y++){
      for(int x=0; x<S; x++) {
	buffer_f = im_in[y][x];
	fwrite(&buffer_f, sizeof(float), 1, output_ptr);
      }
    }
  }

  // close and free
  fclose(output_ptr);
  free_2d(S, im_in);
}

void persist_input_kernels() {
  float **kernels_in = malloc_2d(M, K*K, 0.0);

  FILE *output_ptr;
  int buffer_i;
  float buffer_f;

  for(int m=0; m<M; m++){
    for(int k=0; k<K*K; k++) {
      kernels_in[m][k] = random_float();
    }
  }

  print_kernels(kernels_in, 5);
  
  /* write output file */
  output_ptr = fopen("new_kernels.bin", "wb");
  {
    // header (#kernels, #kernel_size)
    buffer_i = M;
    fwrite(&buffer_i, sizeof(int), 1, output_ptr);

    buffer_i = K;
    fwrite(&buffer_i, sizeof(int), 1, output_ptr);

    // data
    for(int m=0; m<M; m++){
      for(int k=0; k<K*K; k++) {
	       buffer_f = kernels_in[m][k];
	       fwrite(&buffer_f, sizeof(float), 1, output_ptr);
      }
    }
  }
  
  // close and free
  fclose(output_ptr);
  free_2d(M, kernels_in);
}



// READ
float** read_persisted_matrix(char* matrix_file) {
  float **im_out;
  int matrix_dim;
  
  FILE *input_ptr;
  int buffer_i;
  float buffer_f;

  input_ptr = fopen(matrix_file, "rb");

  if (input_ptr == NULL) {
    printf("Failed to open matrix file\n");
    exit(-1);
  }

  // infers image size from header
  fread(&buffer_i, sizeof(int), 1, input_ptr);
  matrix_dim = buffer_i;

  // finally allocates and fills input matrix
  im_out = malloc_2d(matrix_dim, matrix_dim, 0.0);
  
  for(int y=0; y<matrix_dim; y++){
    for(int x=0; x<matrix_dim; x++) {
      fread(&buffer_f, sizeof(float), 1, input_ptr);
      im_out[y][x] = buffer_f;
    }
  }

  // close file and return pointer
  fclose(input_ptr);
  return im_out;
}

float** read_persisted_kernels(char* kernels_file){
  float **kernels_out;
  int n_kernels=0;
  int n_filter=0;
  
  FILE *input_ptr;
  int buffer_i;
  float buffer_f;

  input_ptr = fopen(kernels_file, "rb");
  if (input_ptr == NULL){
    printf("Failed to open kernels file\n");
    exit(-1);
  }

  // infers how many kernels and their size from header
  fread(&buffer_i, sizeof(int), 1, input_ptr);
  n_kernels = buffer_i;

  fread(&buffer_i, sizeof(int), 1, input_ptr);
  n_filter = buffer_i;

  // allocates and fills kernels matrix
  kernels_out = malloc_2d(n_kernels, (n_filter*n_filter), 0.0);

  for(int m=0; m<n_kernels; m++){
    for(int k=0; k<(n_filter*n_filter); k++) {
      fread(&buffer_f, sizeof(float), 1, input_ptr);
      kernels_out[m][k] = buffer_f;
    }
  }

  // close file and return pointer
  fclose(input_ptr);
  return kernels_out;
}

void kernel_matrix_from_line(float *kernel_line, float** kernel_matrix) {
  for(int y=0; y<K; y++) {
    for(int x=0; x<K; x++) {
      kernel_matrix[y][x] = kernel_line[K*y + x];
    }
  }
}
