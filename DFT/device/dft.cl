__kernel void dft(__global float *y,__global float *x) {

  int N = *(&y + 1) - y; //length
  int num_vectors = N / 4;
  float X_real ;
  float X_imag ;
  float4 input, arg, w_real, w_imag;
  int k;
  int i;
  float two_pi_k_over_N;
  
  for (k = 0; k < N/2+1; k++) {
    X_real = 0.0f;
    X_imag = 0.0f;
    two_pi_k_over_N = 2 * M_PI_F * k / N;
    for (i = 0; i < num_vectors; i++) {
      arg = (float4)(two_pi_k_over_N * (i * 4), two_pi_k_over_N * (i * 4 + 1),
                     two_pi_k_over_N * (i * 4 + 2),
                     two_pi_k_over_N * (i * 4 + 3));
      w_real = cos(arg);
      w_imag = sin(arg);
      input = vload4(i, y);

      X_real += dot(input, w_real);
      X_imag -= dot(input, w_imag);
    }

   
    if (k == 0) {
      x[0] = X_real;
    } else if (k == N/2 ) {
      x[1] = X_real;
    } else {
      x[k * 2] = X_real;
      x[k * 2 + 1] = X_imag;
    }
  }
}