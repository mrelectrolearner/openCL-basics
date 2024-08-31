#define WIDTH 800
#define HEIGHT 800
#define MAX_ITER 10000

typedef struct {
    double real;
    double imag;
} Complex;

int mandelbrot(Complex c);

__kernel void generateMandelbrot(__global int *image) {
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

            image[i*size+j] = mandelbrot(c);
        }
    }
}

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
