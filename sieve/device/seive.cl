
__kernel void sieveOfEratosthenes(__global int * restrict limit, __global int * restrict prime) {
           
    for (int i = 0; i <= *limit; i++) {
        prime[i] = 1;  // Initialize all numbers as prime.
    }

    prime[0] = prime[1] = 0;  // 0 and 1 are not prime.

    for (int p = 2; p * p <= *limit; p++) {
        // If prime[p] is not zero, then it is a prime.
        if (prime[p]) {
            // Update all multiples of p to zero
            for (int i = p * p; i <= *limit; i += p) {
                prime[i] = 0;
            }
        }
    }

}