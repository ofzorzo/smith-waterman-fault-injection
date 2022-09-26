CC = gcc
CC_FLAGS = -g -O0
DATA_OBJS = ../data/malloc2d.o ../data/input_synth.o ../data/carolfi_output.o
CC_LIBS = -lm


#
all: linear dft dft2

clean:
	rm *.o *.out -f

clean_data:
	rm ../data/*.bin ../data/*.txt -f

# builds from data/ (does not generate input again)
data_deps:
	cd ../data ; make all

reset_data: clean_data data_deps
	cd ../data ; make reset_data

# implementations builds
linear: linear.c data_deps
	$(CC) $(CC_FLAGS) -I ../data/ $(DATA_OBJS) linear.c $(CC_LIBS) -o linear.out

fft: fft.cpp data_deps
	g++ $(CC_FLAGS) -I ../data/ $(DATA_OBJS) fft.cpp $(CC_LIBS) -o fft.out

dft: dft.c data_deps
	$(CC) $(CC_FLAGS) dft.c $(CC_LIBS) -o dft.out

dft2: dft2.c data_deps
	$(CC) $(CC_FLAGS) -I ../data/ $(DATA_OBJS) dft2.c $(CC_LIBS) -o dft2.out

# implementations execution
run_all: clean run_linear run_dft2

run_linear: linear
	./linear.out ../data/new_matrix.bin ../data/new_kernels.bin output.txt detection.txt
	cat output.txt | wc -l

run_fft: fft
	/usr/bin/time -vo fft.log ./fft.out ../data/new_matrix.bin ../data/new_kernels.bin output.txt detection.txt

run_dft: dft
	./dft.out

run_dft2: dft2
	/usr/bin/time -vo dft2.log ./dft2.out ../data/new_matrix.bin ../data/new_kernels.bin output.txt detection.txt
