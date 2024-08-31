#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#define ROW_A 8
#define COL_A 9
#define ROW_B 12
#define COL_B 9
#define WIDTH 800
#define HEIGHT 800
#define MAX_ITER 10000

typedef struct {
    double real;
    double imag;
} Complex;

int mandelbrot(Complex c) {
    Complex z;
    z.real = 0;
    z.imag = 0;

    int iterations = 0;

    while (iterations < MAX_ITER) {
        double z_real_sq = z.real * z.real;
        double z_imag_sq = z.imag * z.imag;

        if (z_real_sq + z_imag_sq > 4.0) {
            return iterations;
        }

        z.imag = 2 * z.real * z.imag + c.imag;
        z.real = z_real_sq - z_imag_sq + c.real;

        iterations++;
    }

    return iterations;
}
void generateMandelbrot(int image[WIDTH][HEIGHT]) {
    double xmin = -2.0;
    double xmax = 1.0;
    double ymin = -1.5;
    double ymax = 1.5;

    double xstep = (xmax - xmin) / WIDTH;
    double ystep = (ymax - ymin) / HEIGHT;

    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            Complex c;
            c.real = xmin + i * xstep;
            c.imag = ymin + j * ystep;

            image[i][j] = mandelbrot(c);
        }
    }
}

void saveImage(int image[WIDTH][HEIGHT], const char *filename) {
    FILE *fp = fopen(filename, "wb");
    fprintf(fp, "P6\n%d %d\n255\n", WIDTH, HEIGHT);

    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            unsigned char color = (unsigned char)(image[i][j] % 256);
            fwrite(&color, 1, 1, fp);
            fwrite(&color, 1, 1, fp);
            fwrite(&color, 1, 1, fp);
        }
    }

    fclose(fp);
}


void factorial(const int *A,double *C) {
    int i;
    double accumulator=1;

    for(i= 1; i <= *A; i++ ){
        accumulator*=i;
    }

    *C = accumulator;

}


void sieveOfEratosthenes(int limit) {
    // Create a boolean array "prime[0..limit]" and initialize all entries as true.
    // A value in prime[i] will finally be false if i is Not a prime, else true.
    int *prime = (int *)malloc((limit + 1) * sizeof(int));
    for (int i = 0; i <= limit; i++) {
        prime[i] = 1;  // Initialize all numbers as prime.
    }

    prime[0] = prime[1] = 0;  // 0 and 1 are not prime.

    for (int p = 2; p * p <= limit; p++) {
        // If prime[p] is not changed, then it is a prime.
        if (prime[p]) {
            // Update all multiples of p
            for (int i = p * p; i <= limit; i += p) {
                prime[i] = 0;
            }
        }
    }
     // Print all prime numbers
/*     printf("Prime numbers up to %d are: ", limit);
    for (int i = 2; i <= limit; i++) {
        if (prime[i]) {
            printf("%d, ", i);
        }
    }
    printf("\n"); */

    free(prime);

}

void multiplyMatrices(int firstMatrix[ROW_A][COL_A], int secondMatrix[ROW_B][COL_B], int result[ROW_A][COL_B]) {
    for (int i = 0; i < ROW_A; ++i) {
        for (int j = 0; j < COL_B; ++j) {
            result[i][j] = 0;
        }
    }

    for (int i = 0; i < ROW_A; ++i) {
        for (int j = 0; j < COL_B; ++j) {
            for (int k = 0; k < COL_A; ++k) {
                result[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
            }
        }
    }
}
int main(){
    struct timeval start, end;
    int mandelbrotImage[WIDTH][HEIGHT];    
   
    gettimeofday(&start,NULL);
    //sleep(27);
    generateMandelbrot(mandelbrotImage);

    gettimeofday(&end,NULL);
    saveImage(mandelbrotImage, "mandelbrot.ppm");
    long double time_taken;
     time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - 
                              start.tv_usec)) * 1e-6;
    printf("f time= %Lf sec \n", time_taken);

    return 0;
}
