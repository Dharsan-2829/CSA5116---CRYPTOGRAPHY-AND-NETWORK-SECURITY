#include <stdio.h>
#include <string.h>

/* Padding for ECB, CBC and CFB: a 1 bit followed by as few 0 bits as needed (bytes: 0x80, 0x00 ...).
   EVERY message is padded, even when its length is already a multiple of the block size.
   Motivation: the receiver must be able to remove the padding without ambiguity. If a message that already fills
   its last block were sent unpadded, and its last bytes happened to look like padding (0x80 00 00 ...), the receiver
   would wrongly strip real data. Always adding a padding block removes this ambiguity. */
#define BS 8

int pad(const unsigned char *in, int len, unsigned char *out)
{
    memcpy(out, in, len);
    out[len++] = 0x80;
    while (len % BS) out[len++] = 0x00;
    return len;
}

int unpad(unsigned char *buf, int len)
{
    while (len > 0 && buf[len - 1] == 0x00) len--;
    if (len > 0 && buf[len - 1] == 0x80) return len - 1;
    return -1;
}

void show(const char *label, const unsigned char *b, int n)
{
    printf("%s", label);
    for (int i = 0; i < n; i++) printf("%02X ", b[i]);
    printf("\n");
}

int main()
{
    unsigned char out[64];
    const char *tests[] = {"HELLO", "EXACT_8B", "SEVENCH"};

    for (int t = 0; t < 3; t++)
    {
        int len = strlen(tests[t]);
        int n = pad((const unsigned char *)tests[t], len, out);
        printf("Message \"%s\" (%d bytes) -> padded to %d bytes (%d block%s)\n", tests[t], len, n, n / BS, n > BS ? "s" : "");
        show("  padded  : ", out, n);
        int m = unpad(out, n);
        printf("  unpadded: %.*s\n", m, out);
    }

    /* why a full padding block is needed */
    unsigned char tricky[BS] = {'D', 'A', 'T', 'A', 0x80, 0x00, 0x00, 0x00};
    printf("\nA complete block that ends like padding (data = 44 41 54 41 80 00 00 00):\n");
    int m = unpad(tricky, BS);
    printf("  If NOT padded, the receiver strips it and keeps only %d bytes -> real data is lost!\n", m);
    int n = pad(tricky, BS, out);
    m = unpad(out, n);
    printf("  If padded with an extra block (%d bytes sent), the receiver recovers all %d bytes correctly.\n", n, m);
    return 0;
}
