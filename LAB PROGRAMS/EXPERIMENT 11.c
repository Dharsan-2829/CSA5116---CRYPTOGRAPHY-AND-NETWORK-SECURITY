#include <stdio.h>

/* Number of keys of the Playfair cipher.
   Any arrangement of the 25 letters in the 5x5 matrix is a key -> 25! keys.
   Many keys give the same encryption: the matrix is a "torus", so shifting all rows
   cyclically (5 ways) and all columns cyclically (5 ways) gives the same cipher -> 25 equivalent matrices.
   Effective number of unique keys = 25!/25 = 24!                                               */
int main()
{
    long double f = 1, g;
    int i, p;

    for (i = 2; i <= 25; i++) f *= i;
    g = f;
    for (p = 0; g >= 2; p++) g /= 2;
    if (g >= 1.4142) p++;   /* round to nearest power of 2 */
    printf("Total keys 25!               = %.4Le  (approx 2^%d)\n", f, p);

    f /= 25;
    g = f;
    for (p = 0; g >= 2; p++) g /= 2;
    if (g >= 1.4142) p++;
    printf("a) Effectively unique keys   = 25!/25 = 24! = %.4Le  (approx 2^%d)\n", f, p);
    return 0;
}
