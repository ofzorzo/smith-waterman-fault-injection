#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "malloc2d.h"
#include "input_synth.h"

int main() {
  time_t t;

  
  printf("Generating random seed\n");
  time(&t);
  srand((unsigned int) t);
  
  printf("Persisting input matrix\n");
  persist_input_matrix();
  
  printf("Persisting input kernels\n");
  persist_input_kernels();
  
  return 0;
}

