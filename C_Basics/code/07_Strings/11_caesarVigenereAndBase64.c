// Classic string encodings: Caesar and Vigenere ciphers and Base64
// encoding/decoding with bit shifts.

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static void caesar(const char *in, int shift, char *out)
{
    for (; *in; in++, out++)
    {
        if (isupper((unsigned char)*in)) *out = (char)('A' + (*in - 'A' + shift + 26) % 26);
        else if (islower((unsigned char)*in)) *out = (char)('a' + (*in - 'a' + shift + 26) % 26);
        else *out = *in;
    }
    *out = '\0';
}

static void vigenere(const char *in, const char *key, int decrypt, char *out)
{
    int k = 0, klen = (int)strlen(key);
    for (; *in; in++, out++)
    {
        if (isalpha((unsigned char)*in))
        {
            int shift = toupper((unsigned char)key[k++ % klen]) - 'A';
            if (decrypt) shift = 26 - shift;
            char base = isupper((unsigned char)*in) ? 'A' : 'a';
            *out = (char)(base + (*in - base + shift) % 26);
        }
        else *out = *in;
    }
    *out = '\0';
}

static const char B64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static void base64_encode(const unsigned char *in, int n, char *out)
{
    int o = 0;
    for (int i = 0; i < n; i += 3)
    {
        unsigned v = in[i] << 16 | (i + 1 < n ? in[i + 1] << 8 : 0) | (i + 2 < n ? in[i + 2] : 0);   /* 24 bits -> four 6-bit groups */
        out[o++] = B64[v >> 18 & 63];
        out[o++] = B64[v >> 12 & 63];
        out[o++] = i + 1 < n ? B64[v >> 6 & 63] : '=';
        out[o++] = i + 2 < n ? B64[v & 63] : '=';
    }
    out[o] = '\0';
}

static int base64_decode(const char *in, unsigned char *out)
{
    int o = 0, bits = 0;
    unsigned acc = 0;
    for (; *in && *in != '='; in++)
    {
        acc = acc << 6 | (unsigned)(strchr(B64, *in) - B64);
        bits += 6;
        if (bits >= 8)
        {
            bits -= 8;
            out[o++] = (unsigned char)(acc >> bits & 255);
        }
    }
    return o;
}

int main(void)
{
    char out[64], back[64];
    caesar("Hello, World!", 3, out);
    caesar(out, -3, back);
    printf("caesar +3: %s -> back: %s\n", out, back);
    vigenere("ATTACKATDAWN", "LEMON", 0, out);
    vigenere(out, "LEMON", 1, back);
    printf("vigenere: %s -> back: %s\n", out, back);

    char enc[64];
    unsigned char dec[64];
    base64_encode((const unsigned char *)"Many hands make light work.", 27, enc);
    int n = base64_decode(enc, dec);
    dec[n] = '\0';
    printf("base64: %s\ndecoded: %s\n", enc, dec);
    base64_encode((const unsigned char *)"Ma", 2, enc);
    printf("padding for two bytes: %s\n", enc);
    return 0;
}
