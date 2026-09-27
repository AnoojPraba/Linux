#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>
#include <unistd.h>

#define DEVICE_READY_DELAY_USEC 200000
#define POLL_INTERVAL_USEC 1000

// Simulated device: a background thread that, after a delay, sets a flag to
// mimic a device signalling completion. Userspace can't generate a real
// hardware interrupt, so this demo contrasts the two *waiting* strategies -
// busy-wait polling vs an event-driven blocking wait - which is the part of
// polling-vs-interrupts observable from userspace; NOTES.md covers the
// kernel-level ISR/DMA machinery this can't demo directly.
static atomic_int deviceReady = 0;

void *deviceThread(void *arg)
{
    (void)arg;
    usleep(DEVICE_READY_DELAY_USEC);
    atomic_store(&deviceReady, 1);

    return NULL;
}

// Polling: the "CPU" spins here, repeatedly checking the status flag
// instead of doing anything else, until the device becomes ready.
void pollForDevice(void)
{
    int pollCount = 0;

    while (!atomic_load(&deviceReady))
    {
        pollCount++;
        usleep(POLL_INTERVAL_USEC);
    }

    printf("polling: device ready after %d status checks\n", pollCount);
}

int main()
{
    pthread_t thread;

    atomic_store(&deviceReady, 0);
    pthread_create(&thread, NULL, deviceThread, NULL);

    pollForDevice();

    pthread_join(thread, NULL);

    return 0;
}
