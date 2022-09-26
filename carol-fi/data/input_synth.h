#define S 256
#define K 3
#define M 256

// VISUALIZATION
void print_matrix(float**, int, int);
void print_kernels(float**, int);

// WRITE
float random_float();
void persist_input_matrix();
void persist_input_kernels();

//READ
float** read_persisted_matrix(char*);
float** read_persisted_kernels(char*);
void kernel_matrix_from_line(float *kernel_line, float **kernel_matrix);
