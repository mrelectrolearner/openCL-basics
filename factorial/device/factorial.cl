__kernel void factorial(__global const int *A,__global double *C) {
    int i;
    int accumulator=1;

    for(i= 1; i <= *A; i++ ){
        accumulator*=i;
    }

    *C = accumulator;

}