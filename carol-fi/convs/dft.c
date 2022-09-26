#include <stdio.h>
#include <math.h>

#define PI 3.14159265
#define S 4 // spatial resolution (input size)
#define K 3 // kernel size
#define N (S+K-1)

// NOT INJECTING FAULTS HERE!!

void dft(int n_samples, float* x, float* y_real, float* y_im) {
  for(int k=0; k<n_samples; k++) {
    for (int n=0; n<n_samples; n++) {
      y_real[k] += x[n] * cos(2*PI*k*n/N);
      y_im[k] -= x[n] * sin(2*PI*k*n/N);
    }
  }
}

void idft(int n_samples, float* y, float* y_real, float* y_im) {

  for (int n=0; n<n_samples; n++) {
    for(int k=0; k<n_samples; k++) {
      y[n] += y_real[k] * cos(2*PI*k*n/N);
      y[n] -= y_im[k] * sin(2*PI*k*n/N);
    }
    y[n] /= (float) n_samples;
  }
}

void complex_mul(int n_samples, float* a_real, float* a_im, float* b_real, float* b_im, float* out_real, float* out_im) {
  for(int n=0; n<n_samples; n++) {
    out_real[n] = 0;
    out_real[n] += a_real[n] * b_real[n];
    out_real[n] -= a_im[n] * b_im[n];

    out_im[n] = 0;
    out_im[n] += a_im[n] * b_real[n];
    out_im[n] += a_real[n] * b_im[n];
  }

}

int main () {
  // Instantiate input signal (spatial data, vectors for frequency coefficients, reversed spatial data)
  float x[N] = {2, 1, 2, 1};
  float fx_real[N] = {0};
  float fx_im[N] = {0};
  float x_[N] = {0};

  // Instantiate signal filter (same as input signal)
  float k[N] = {1, 2, 3};
  float fk_real[N] = {0};
  float fk_im[N] = {0};
  float k_[N] = {0};

  // Instantiate filter output (vectors for convolution output as frequency coefficientes, reversed spatial data as final output)
  float fy_real[N] = {0};
  float fy_im[N] = {0};
  float y_[N] = {0};


  printf("\nInput\n");
  {
    for(int i=0; i<N; i++) printf("X: %f ", x[i]); printf("\n");
    for(int i=0; i<N; i++) printf("K: %f ", k[i]); printf("\n");
  }

  // Space -> Frequency
  dft(N, x, fx_real, fx_im);
  dft(N, k, fk_real, fk_im);

  // Point-wise multiplication
  complex_mul(N, fx_real, fx_im, fk_real, fk_im, fy_real, fy_im);

  printf("\nEncoded\n");

  {
    for(int i=0; i<N; i++) printf("F(X)_r: %f ", fx_real[i]); printf("\n");
    for(int i=0; i<N; i++) printf("F(X)_i: %f ", fx_im[i]); printf("\n");
    /*
    for(int i=0; i<N; i++) printf("F(K)_cos: %f ", fk_cos[i]); printf("\n");
    for(int i=0; i<N; i++) printf("F(K)_sin: %f ", fk_sin[i]); printf("\n");
    for(int i=0; i<N; i++) printf("F(Y)_cos: %f ", fy_cos[i]); printf("\n");
    for(int i=0; i<N; i++) printf("F(Y)_sin: %f ", fy_sin[i]); printf("\n");
    */

  }


  // Frequency -> Space
  idft(N, x_, fx_real, fx_im);
  idft(N, y_, fy_real, fy_im);
  printf("\nDecoded\n");
  {
    for(int i=0; i<N; i++) printf("Xf: %f ", x_[i]); printf("\n");
    for(int i=0; i<N; i++) printf("Yf: %f ", y_[i]); printf("\n");
  }



  printf("\ndeix \n");
  return(0);
}
