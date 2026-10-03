#include <stdio.h>

/* RSA: public key e = 31, n = 3599. Find the private key d.
   1) trial and error:  n = p * q
   2) phi(n) = (p-1)(q-1)
   3) d = e^-1 mod phi(n) using the extended Euclidean algorithm                               */
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
    long long e = 31, n = 3599, p = 0, q = 0, x, y;

    printf("Public key: e = %lld, n = %lld\n", e, n);
    for (p = 2; p * p <= n; p++)
        if (n % p == 0) { q = n / p; break; }
    printf("Trial and error: n = %lld x %lld\n", p, q);

    long long phi = (p - 1) * (q - 1);
    printf("phi(n) = (%lld-1)(%lld-1) = %lld\n", p, q, phi);

    egcd(e, phi, &x, &y);
    long long d = ((x % phi) + phi) % phi;
    printf("Extended Euclid: %lld*(%lld) + %lld*(%lld) = 1\n", e, x, phi, y);
    printf("Private key d = %lld   (check: e*d mod phi = %lld)\n", d, (e * d) % phi);

    long long m = 1234, c = modpow(m, e, n);
    printf("\nTest: message %lld -> cipher %lld -> decrypted %lld\n", m, c, modpow(c, d, n));
    return 0;
}
