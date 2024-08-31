__kernel void vector_add(__global const int *A,  __global int *C) {
 

    int i;

    for(i=0;i<10;i++){
        C[i] = A[i]+40;
    }
    
}