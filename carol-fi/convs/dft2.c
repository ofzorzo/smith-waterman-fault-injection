#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "malloc2d.h"
#include "input_synth.h" // yields print_matrix, S, K
#include "carolfi_output.h"

#define N (S+K-1)
#define PI 3.14159265

void dft2(float **in, float **out_real, float **out_im) {
  float mid_real[N][N] = {0};
  float mid_im[N][N] = {0} ;

  // rows first
  for(int y=0; y<N; y++) {
    for(int k=0; k<N; k++) {
      for(int n=0; n<N; n++) {
	      mid_real[y][k] += in[y][n] * cos(2*PI*k*n/N);
	      mid_im[y][k] -= in[y][n] * sin(2*PI*k*n/N);
      }
    }
  }

  // then columns
  for(int x=0; x<N; x++) {
    for(int n=0; n<N; n++) {
      for(int k=0; k<N; k++) {
	out_real[k][x] += mid_real[n][x] * cos(2*PI*k*n/N);
	out_im[k][x] -= mid_real[n][x] * sin(2*PI*k*n/N);

	out_real[k][x] += mid_im[n][x] * sin(2*PI*k*n/N);
	out_im[k][x] += mid_im[n][x] * cos(2*PI*k*n/N);
      }
    }
  }
}

void idft2(float **in_real, float **in_im, float **out) {
  float mid_real[N][N] = {0};
  float mid_im[N][N] = {0};

  // rows first
  for(int y=0; y<N; y++) {
    for(int k=0; k<N; k++) {
      for(int n=0; n<N; n++) {
	mid_real[y][k] += in_real[y][n] * cos(2*PI*k*n/N);
	mid_im[y][k] += in_real[y][n] * sin(2*PI*k*n/N);

	mid_real[y][k] -= in_im[y][n] * sin(2*PI*k*n/N);
	mid_im[y][k] += in_im[y][n] * cos(2*PI*k*n/N);
      }
    }
  }

  // then columns
  for(int x=0; x<N; x++) {
    for(int n=0; n<N; n++) {
      for(int k=0; k<N; k++) {
	out[n][x] += mid_real[k][x] * cos(2*PI*k*n/N);
	out[n][x] -= mid_im[k][x] * sin(2*PI*k*n/N);
      }
      out[n][x] /= N*N;
    }
  }
}

void complex_mul2(float **a_real, float **a_im, float **b_real, float **b_im, float **out_real, float **out_im) {
  for(int m=0; m<N; m++) {
    for(int n=0; n<N; n++) {
      out_real[m][n] = 0;
      out_real[m][n] += a_real[m][n] * b_real[m][n];
      out_real[m][n] -= a_im[m][n] * b_im[m][n];

      out_im[m][n] = 0;
      out_im[m][n] += a_im[m][n] * b_real[m][n];
      out_im[m][n] += a_real[m][n] * b_im[m][n];
    }
  }
}

void shift2d(float** in, float **out, int limit_in, int limit_out) {
  int mean_point = limit_in/2;
  int size_diff = limit_out - limit_in;

  // 3 -> 1
  for(int i=mean_point; i<limit_in; i++)
    for(int j=mean_point; j<limit_in; j++)
      out[i-mean_point][j-mean_point] = in[i][j];

  // 1 -> 3
  for(int i=0; i<mean_point; i++)
    for(int j=0; j<mean_point; j++)
      out[i+mean_point+size_diff+1][j+mean_point+size_diff+1] = in[i][j];

  // ...
  for(int i=mean_point; i<limit_in; i++)
    for(int j=0; j<mean_point; j++)
      out[i-mean_point][j+mean_point+size_diff+1] = in[i][j];

  for(int i=0; i<mean_point; i++)
    for(int j=mean_point; j<limit_in; j++)
      out[i+mean_point+size_diff+1][j-mean_point] = in[i][j];
}

void conv_wrapper(float **input_matrix, float *input_kernel, float **y_) {
  // Input signal (spatial data, vectors for frequency coefficients, reversed spatial data)
  float **x;
  float **x_real;
  float **x_im;
  float **x_;


  // Filter signal (same as input signal)
  float **k;
  float **k_shifted;
  float **k_real;
  float **k_im;

  // Filter output (vectors for convolution output as frequency coefficients, reversed spatial data as final output)
  float **y_real;
  float **y_im;

  int verbose = 0;


  // Allocates (a lot of) memory
  x = malloc_2d(N, N, 0.0);
  x_real = malloc_2d(N, N, 0.0);
  x_im = malloc_2d(N, N, 0.0);
  x_ = malloc_2d(N, N, 0.0);

  k = malloc_2d(N, N, 0.0);
  k_shifted = malloc_2d(N, N, 0.0);
  k_real = malloc_2d(N, N, 0.0);
  k_im = malloc_2d(N, N, 0.0);

  y_real = malloc_2d(N, N, 0.0);
  y_im = malloc_2d(N, N, 0.0);


  // Fills input matrix (implicit zero-padding)
  for (int y=0; y<S; y++)
    for(int ix=0; ix<S; ix++)
      x[y][ix] = input_matrix[y][ix];

  // Fills input filter (implicit zero-padding again)
  kernel_matrix_from_line(input_kernel, k);




  if (verbose>0) {
    printf("\nInput matrix\n");
    print_matrix(x, 10, 10);
    printf("\nInput kernel\n");
    print_matrix(k, K, K);
  }


  // Shift filter signal
  shift2d(k, k_shifted, K, N);

  if (verbose>0) {
    printf("\nShifted kernel\n");
    print_matrix(k_shifted, N, 10);
  }


  dft2(x, x_real, x_im);
  dft2(k_shifted, k_real, k_im);

  if (verbose>1) {
    printf("\nEncoded\n");
    print_matrix(x_real, 10, 10);
    print_matrix(x_im, 10, 10);
  }

  complex_mul2(x_real, x_im, k_real, k_im, y_real, y_im);

  if (verbose>1) {
    printf("\nFiltered\n");
    print_matrix(y_real, 10, 10);
    print_matrix(y_im, 10, 10);
  }


  idft2(x_real, x_im, x_);
  idft2(y_real, y_im, y_);

  if (verbose>0) {
    printf("\nDecoded\n");
    print_matrix(x_, 10, 10);
    print_matrix(y_, 10, 10);
  }

  // Frees
  free_2d(N, x);
  free_2d(N, x_im);
  free_2d(N, x_real);
  free_2d(N, x_);

  free_2d(N, k);
  free_2d(N, k_shifted);
  free_2d(N, k_im);
  free_2d(N, k_real);

  free_2d(N, y_im);
  free_2d(N, y_real);


}

int main(int argc, char **argv) {
  // IO buffers
  FILE* output_file_ptr;

  float **input_matrix;
  float **input_kernels;

  float **y_1, **y_2;

  // Checks for correction in arguments
  if(argc == 5) {
    output_file_ptr = fopen(argv[3], "w");
    if (output_file_ptr == NULL) {
      printf("Failed to open output file! \n");
      return -1;
    }
  }
  else {
    printf("Wrong call of script! Usage is '<matrix_file, kernels_file, output_file, detection_file>\n");
    return -1;
  }

  // Reads input
  input_matrix = read_persisted_matrix(argv[1]);
  input_kernels = read_persisted_kernels(argv[2]);

  // Allocates output
  y_1 = malloc_2d(N, N, 0.0);
  y_2 = malloc_2d(N, N, 0.0);

  // Op
  for (int m=0; m < M; m++) {


    conv_wrapper(input_matrix, input_kernels[m], y_1);
    conv_wrapper(input_matrix, input_kernels[m], y_2);

    compare_output(y_1, y_2, argv[4], S);

    output_matrix(output_file_ptr, y_1, S, S);
  }




  // Frees allocated instances
  free_2d(S, input_matrix); // try with N
  free_2d(M, input_kernels);
  free_2d(N, y_1);
  free_2d(N, y_2);

  fclose(output_file_ptr);

  return 0;

}
