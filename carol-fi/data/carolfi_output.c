#include "carolfi_output.h"

void compare_output(float** image1, float** image2, char* detect_ptr_file, int ImgSize) {
  FILE* fp;
  int imgX, imgY;

  union fp_bit_twiddler {
    float f;
    unsigned int i;
  } o1, o2;

  unsigned int xor;

  int byte_length; // might skip sign bit if is zero

  /*
  printf("Storage size for int : %d \n", sizeof(int));
  printf("Storage size for float : %d \n", sizeof(float));
  printf("Storage size for long : %d \n", sizeof(long));
  */


  for (imgX=0; imgX<ImgSize; imgX++)  {
    for(imgY=0; imgY<ImgSize; imgY++) {
      o1.f = image1[imgX][imgY];
      o2.f = image2[imgX][imgY];

      //if (o1.f == 0.0 || o2. == 0.0) {

      //}

      int b=0;
      xor = o1.i ^ o2.i;
      while(xor%2==0 && b < sizeof(unsigned int)*8) {
        xor >>= 1;
        b++;
      }

      if (xor % 2 == 1){
        if (fp = fopen(detect_ptr_file, "a")) {
          fprintf(fp, "%dx%d %.6fv%.6f\n", imgX, imgY, image1[imgX][imgY],image2[imgX][imgY]);
          //printf("%d x %d @ %d / %d -> %.6f vs %.6f - %d\n", imgX, imgY, b, sizeof(unsigned int)*8, image1[imgX][imgY],image2[imgX][imgY], xor%2);
          fclose(fp);
        }
      }
    }
  }
}

void output_matrix(FILE* output_ptr, float** matrix, int n2, int n1) {
  float buffer_f;
  for(int y=0; y<n2; y++) {
    for(int x=0; x<n1; x++) {
      buffer_f = matrix[y][x];
      fwrite(&buffer_f, sizeof(float), 1, output_ptr);
    }
  }
}
