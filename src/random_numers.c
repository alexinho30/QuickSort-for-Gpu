void init_array_random_uniform(int* vec){

}


/*
15

Let's assume that rand() generates a uniformly-distributed value I in the range [0..RAND_MAX], and you want to generate a uniformly-distributed value O in the range [L,H].

Suppose I in is the range [0..32767] and O is in the range [0..2].

According to your suggested method, O= I%3. Note that in the given range, there are 10923 numbers for which I%3=0, 10923 number for which I%3=1, but only 10922 number for which I%3=2. Hence your method will not map a value from I into O uniformly.

As another example, suppose O is in the range [0..32766].

According to your suggested method, O=I%32767. Now you'll get O=0 for both I=0 and I=32767. Hence 0 is twice as likely than any other value - your method is again nonuniform.

The suggest way to generate a uniform mapping is as follow:

Calculate the number of bits that are needed to store a random value in the range [L,H]:

unsigned int nRange = (unsigned int)H - (unsigned int)L + 1;
unsigned int nRangeBits= (unsigned int)ceil(log((double(nRange) / log(2.));

Generate nRangeBits random bits

this can be easily implemented by shifting-right the result of rand()

Ensure that the generated number is not greater than H-L. If it is - repeat step 2.

Now you can map the generated number into O just by adding a L


*/