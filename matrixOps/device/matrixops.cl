__kernel void matrixMul(__global const double* a, 
                            __global const double* b, 
                            __global double* result, 
                                    const int size) { 
                int row = get_global_id(0); 
                int col = get_global_id(1); 
                int sum = 0; 
                for (int i = 0; i < size; ++i) { 
                    sum += a[row * size + i] * b[i * size + col]; 
                }       
                result[row * size + col] = sum; 
    }