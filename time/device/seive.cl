__kernel void sieveOfEratosthenes(__global int * restrict limit, __global int * restrict prime) {
    int element= get_global_id(0); 
    
    if(element==0 ||element==1) prime[element] = 0;  // 0 and 1 are not prime.

    if(element>1) {
        // If prime[p] is not zero, then it is a prime.
        if (prime[element]) {
            // Update all multiples of p to zero
            for (int i = element * element; i <= *limit; i += element) {
                prime[i] = 0;
            }
        }
    }

}