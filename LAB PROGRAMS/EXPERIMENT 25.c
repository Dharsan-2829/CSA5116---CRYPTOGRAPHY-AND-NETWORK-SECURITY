#include <stdio.h>

/* RSA: if a plaintext block M has a common factor with n = pq (say p | M), then the ciphertext C = M^e mod n
   is also divisible by p. So gcd(C, n) = p reveals a factor of n, n is factored, phi(n) and the private key
   d follow, and ALL blocks can be decrypted. YES - it helps the attacker completely.
   Demo: n = 3599, e = 31, block M = 118 = 2*59 (59 divides n).                                        */
long long gcd(long long a, long long b) { return b ? gcd(b, a % b) : a; }

long long egcd(long long a, long long b, long long *x, long long *y)
{
    if (b == 0) { *x = 1; *y = 0; return a; }
    long long x1, y1, g = egcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

long long modpow(long long b, long long e, long long m)
{
    long long r = 1;
    b %= m;
    while (e) { if (e & 1) r = r * b % m; b = b * b % m; e >>= 1; }
    return r;
}

int main()
{
    long long n, e, c, x, y;

    printf("Enter n, e and the ciphertext block C that is known to share a factor with n (e.g. 3599 31 %lld): ", modpow(118, 31, 3599));
    scanf("%lld %lld %lld", &n, &e, &c);

    long long p = gcd(c, n);
    if (p == 1 || p == n)
    {
        printf("gcd(C, n) = %lld -> no factor found.\n", p);
        return 0;
    }
    long long q = n / p;
    printf("gcd(C, n) = %lld  ->  n = %lld x %lld  (n is factored!)\n", p, p, q);

    long long phi = (p - 1) * (q - 1);
    egcd(e, phi, &x, &y);
    long long d = ((x % phi) + phi) % phi;
    printf("phi(n) = %lld, private key d = %lld\n", phi, d);
    printf("Every other block can now be decrypted, e.g. block C decrypts to %lld\n", modpow(c, d, n));
    return 0;
}
