#include <time.h>
#include <stdio.h>

#include "malloc2d.h"
#include "input_synth.h"
#include "carolfi_output.h"

#define ImgSize S
#define KernelSize K
#define NumberOfKernels M

#define _DUPLICATE


void simpleConvolution(float** inputImage, float** outputImage, float** convKernel) {
  int imgX = 0, imgY = 0;

  int kernelX = 0, kernelY = 0;
  int kernelMiddle = (int) (KernelSize/2);

  float outputPixel = 0;

  for(imgX;imgX<ImgSize;imgX++){
    imgY = 0;
    for(imgY;imgY<ImgSize;imgY++){
      outputPixel = 0;

      int kernelImgX = imgX - kernelMiddle;
      int kernelImgY = imgY - kernelMiddle;
      //printf("kernel_image_x %d , kernel_image_y %d \n", kernelImgX, kernelImgY);


      for(kernelX=0;kernelX<KernelSize;kernelX++){
	       for(kernelY=0;kernelY<KernelSize;kernelY++){
	          if((kernelX+kernelImgX >= 0 ) && (kernelY+kernelImgX >= 0 ) && (kernelX+kernelImgX) < ImgSize && (kernelY+kernelImgY) < ImgSize){
	             outputPixel += inputImage[kernelImgX+kernelX][kernelImgY+kernelY] * convKernel[kernelX][kernelY];
            }
         }
		  }
      outputImage[imgX][imgY] = outputPixel;
    }
  }
}
void printImg(float** image) {
  int imgX, imgY;
  for (imgX=0; imgX<ImgSize; imgX++)
  {
    for(imgY=0; imgY<ImgSize; imgY++)
      {
	printf("%f ", image[imgX][imgY]);
      }
    printf("\n");
  }
  printf("\n");
}

int main(int argc, char **argv) {
  float **inputImage;
  float **outputImage1;
  #ifdef _DUPLICATE
    float **outputImage2;
  #endif

  float **inputKernels;
  float **convKernel;

  FILE* output_file_ptr;

  if(argc == 5) {
    output_file_ptr = fopen(argv[3], "w");
    if (output_file_ptr == NULL) {
      printf("Failed to open output file! \n");
      return -1;
    }

    inputImage = read_persisted_matrix(argv[1]);
    inputKernels = read_persisted_kernels(argv[2]);

    convKernel = malloc_2d(KernelSize, KernelSize, 5.0);

    outputImage1 = malloc_2d(ImgSize, ImgSize, -1.0);
    #ifdef _DUPLICATE
      outputImage2 = malloc_2d(ImgSize, ImgSize, -1.0);
    #endif

  }
  else {
    printf("Wrong call of script! Usage is '<matrix_file, kernels_file, output_file, detect_file>\n");
    return -1;
  }

  for (int kernelNumber=0; kernelNumber<NumberOfKernels; kernelNumber++){
    kernel_matrix_from_line(inputKernels[kernelNumber], convKernel);


    simpleConvolution(inputImage, outputImage1, convKernel);

    #ifdef _DUPLICATE
    simpleConvolution(inputImage, outputImage2, convKernel);

    compare_output(outputImage1, outputImage2, argv[4], S);
    #endif

    output_matrix(output_file_ptr, outputImage1, ImgSize, ImgSize);
  }

  fclose(output_file_ptr);

  free_2d(ImgSize, inputImage);
  free_2d(NumberOfKernels, inputKernels);

  free_2d(KernelSize, convKernel);
  free_2d(ImgSize, outputImage1);
  #ifdef _DUPLICATE
  free_2d(ImgSize, outputImage2);
  #endif


}
