#include <stdio.h>

#include <complex>
#include <iostream>
#include <valarray>

#include <string.h>
#include <float.h>

#include "input_synth.h"

const int N = S+K-1; //padding
const double PI = 3.141592653589793238460;

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;
typedef std::valarray<CArray> KernelsArray;

// Cooley–Tukey FFT (in-place, divide-and-conquer)
// Higher memory requirements and redundancy although more intuitive
void fft(CArray& x){
    const size_t N_size = x.size();
    if (N_size <= 1) return;

    // divide
    CArray even = x[std::slice(0, N_size/2, 2)];
    CArray  odd = x[std::slice(1, N_size/2, 2)];

    // conquer
    fft(even);
    fft(odd);

    // combine
    for (size_t k = 0; k < N_size/2; ++k)
    {
        Complex t = std::polar(1.0, -2 * PI * k / N_size) * odd[k];
        x[k    ] = even[k] + t;
        x[k+N_size/2] = even[k] - t;
    }
}

// inverse fft (in-place)
void ifft(CArray& x){
    // conjugate the complex numbers
    x = x.apply(std::conj);

    // forward fft
    fft( x );

    // conjugate the complex numbers again
    x = x.apply(std::conj);

    // scale the numbers
    x /= x.size();
}

//
void print_carray(CArray& X, int size) {
  std::cout << '\n';

  for(int i=0; i<size; i++)
    std::cout << '|' << X[i] << '\t';

  std::cout << "|\n";
}

void compare_output(CArray& line1, CArray& line2, char* detect_ptr_file, int line_size) {
  FILE* fp;
  int line_i;

  union fp_bit_twiddler {
    float f;
    unsigned int i;
  } o1, o2;

  unsigned int i_xor;

  int byte_length; // might skip sign bit if is zero

  /*
  printf("Storage size for int : %d \n", sizeof(int));
  printf("Storage size for float : %d \n", sizeof(float));
  printf("Storage size for long : %d \n", sizeof(long));
  */


  for(line_i=0; line_i<line_size; line_i++) {
    o1.f = line1[line_i].real();
    o2.f = line2[line_i].real();


    int b=0;
    i_xor = o1.i ^ o2.i;
    while(i_xor%2==0 && b < sizeof(unsigned int)*8) {
      i_xor >>= 1;
      b++;
    }

    if (i_xor % 2 == 1){
      if (fp = fopen(detect_ptr_file, "a")) {
        fprintf(fp, "%d %.6fv%.6f\n", line_i, (float)line1[line_i].real(), (float)line2[line_i].real());
        //printf("%d x %d @ %d / %d -> %.6f vs %.6f - %d\n", imgX, imgY, b, sizeof(unsigned int)*8, image1[imgX][imgY],image2[imgX][imgY], i_xor%2);
        fclose(fp);
      }
    }
  }

}


/*
int compare_output(CArray& line1, CArray& line2, char* detect_ptr_file) {
  FILE* fp;
  int i;

  char val1[50];
  char val2[50];

  for (i=0; i<S; i++)

      if (isnan((line1[i].real())) ||
          isnan((line2[i].real())) ||
          ((float)line1[i].real() != (float)line2[i].real()))
        if (fp = fopen(detect_ptr_file, "a")) {
          fprintf(fp, "[%d]: %f %f\n", i, line1[i].real(), line2[i].real());
          fclose(fp);
        }
}
*/

void read_persisted_line(CArray& X, char *matrix_file) {
  int matrix_dim;

  FILE *input_ptr;
  int buffer_i;
  float buffer_f;

  input_ptr = fopen(matrix_file, "rb");

  if (input_ptr == NULL) {
    printf("Failed to open matrix file\n");
    exit(-1);
  }

  // infers line size from header
  fread(&buffer_i, sizeof(int), 1, input_ptr);
  matrix_dim = buffer_i;

  //std::cout << matrix_dim << " samples per line \n";

  // reads first line
  for(int x=0; x<matrix_dim; x++) {
    fread(&buffer_f, sizeof(float), 1, input_ptr);
    X[x] = (Complex) buffer_f;
  }


  fclose(input_ptr);
}

void read_persisted_kernels(KernelsArray& kernels, char *kernels_file) {
  int n_kernels=0;
  int filter_dim=0;

  FILE* input_ptr;
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
  filter_dim = buffer_i;

  //std::cout << n_kernels << " kernels and " << filter_dim << " samples per kernel \n";

  for(int m=0; m<n_kernels; m++){
    kernels[m] = CArray(0.0, filter_dim);
    for(int k=0; k<filter_dim; k++) {
      fread(&buffer_f, sizeof(float), 1, input_ptr);

      kernels[m][k] = (Complex) buffer_f;
      //std::cout << buffer_f << '\n';
    }

  }

  fclose(input_ptr);
}

void output_line(FILE* output_ptr, CArray line, int size, int verbosity) {
  float buffer_f;
  for(int x=0; x<size; x++) {
    buffer_f = (float)line[x].real();
    fwrite(&buffer_f, sizeof(float), 1, output_ptr);
  }

  if (verbosity > 0) print_carray(line, size);
}


void conv_wrapper(CArray& raw_input, CArray& raw_kernel, CArray& output, FILE* output_file_ptr, int do_output) {
  CArray input(0.0, N);
  CArray filter(0.0, N);

  // Buffers input
  for (int i=0; i<N; i++)
    input[i] = raw_input[i];

  // copy persisted kernel into filter buffer (zeroes and shifts)
  for(int k=0; k<N; k++)
    filter[k] = 0.0;
  for(int k=0; k<K/2; k++)
    filter[k + (N - K/2)] = raw_kernel[k];
  for(int k=K/2; k<K; k++)
    filter[k - K/2] = raw_kernel[k];

  // forward fft
  fft(input);
  fft(filter);

  // multiplication
  for (int i=0; i<N; ++i) {
    output[i] = input[i] * filter[i];
  }

  // inverse fft
  //ifft(input);
  //ifft(filter);
  ifft(output);

  if (do_output)
    output_line(output_file_ptr, output, S, 0);
}

int main(int argc, char **argv)
{

  FILE* output_file_ptr;

  char* matrix_file = argv[1];
  char* kernels_file = argv[2];
  char* output_file = argv[3];

  // Instantiates
  CArray raw_input(0.0, N);
  KernelsArray kernels(M);


  CArray output1(0.0, N);
  CArray output2(0.0, N);



  // Opens output file and reads persisted input
  if(argc == 5) {
    output_file_ptr = fopen(output_file, "w");
    if (output_file_ptr == NULL) {
      printf("Failed to open output file! \n");
      return -1;
    }
  }
  else {
    printf("Wrong call of script! Usage is '<matrix_file, kernels_file, output_file, detect_file>\n");
    return -1;
  }

  read_persisted_line(raw_input, matrix_file);
  read_persisted_kernels(kernels, kernels_file);

  for (int m=0; m<M; m++) {
    conv_wrapper(raw_input, kernels[m], output1, output_file_ptr, 1);
    conv_wrapper(raw_input, kernels[m], output2, output_file_ptr, 0);


    compare_output(output1, output2, argv[4], S);
  }


  fclose(output_file_ptr);


  return 0;
}
