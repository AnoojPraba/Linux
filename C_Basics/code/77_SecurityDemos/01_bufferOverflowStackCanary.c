#include <stdio.h>
#include <string.h>

// A classic stack buffer overflow: copying attacker-controlled data into a
// fixed-size stack buffer with no bounds check. Historically this let an
// attacker overwrite the saved return address just past the buffer,
// redirecting execution to code of their choosing when the function
// returned - the root cause behind a large fraction of historical memory-
// corruption CVEs.
__attribute__((noinline))
void vulnerableCopy(const char *input)
{
    char buffer[16];

    // strcpy has no length limit - it copies until it hits input's own
    // '\0', writing past "buffer" if input is longer than 16 bytes,
    // corrupting whatever the compiler placed next on the stack (other
    // locals, saved registers, or - historically - the return address).
    strcpy(buffer, input);
    printf("buffer contents: %s\n", buffer);
}

int main()
{
    // A stack canary (-fstack-protector[-strong], enabled by default on
    // many distros' gcc, but NOT this toolchain's default - compile this
    // file with -fstack-protector-all to see it) places a random value
    // right after local buffers, checked just before the function
    // returns. If this overflow reaches into that canary and corrupts it
    // (which a 40-byte input into a 16-byte buffer reliably does), the
    // check fails and the program calls __stack_chk_fail(), aborting with
    // "*** stack smashing detected ***" instead of letting a corrupted
    // return address run. Without the flag, this same overflow instead
    // produces a raw, less informative SIGSEGV (or worse - silent memory
    // corruption with no crash at all) - the canary can't prevent the
    // overflow itself, only detect it before it's exploited.
    const char *maliciousInput = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

    printf("about to overflow a 16-byte buffer with a %zu-byte input...\n",
           strlen(maliciousInput));
    vulnerableCopy(maliciousInput);

    printf("this line only prints if the canary wasn't corrupted\n");
    return 0;
}
