#include <stdio.h>

#define MAX_LISTENERS 4

typedef void (*EventCallback)(void *userData);

typedef struct
{
    EventCallback callback;
    void *userData;
} Listener;

typedef struct
{
    Listener listeners[MAX_LISTENERS];
    int count;
} EventSource;

/*****************************************************************************
 * Name: eventSourceInit
 *
 * Description:
 *         Initializes an EventSource to have no registered listeners.
 *
 * Inputs:
 *         source : the EventSource to initialize.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void eventSourceInit(EventSource *source)
{
    source->count = 0;
}

/*****************************************************************************
 * Name: eventSourceRegister
 *
 * Description:
 *         Registers a callback and its opaque user data with an EventSource,
 *         if space remains in the listener table.
 *
 * Inputs:
 *         source   : the EventSource to register with.
 *         callback : function invoked when the event fires.
 *         userData : opaque pointer passed back to the callback unchanged.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void eventSourceRegister(EventSource *source, EventCallback callback, void *userData)
{
    if (source->count < MAX_LISTENERS)
    {
        source->listeners[source->count].callback = callback;
        source->listeners[source->count].userData = userData;
        source->count++;
    }
}

/*****************************************************************************
 * Name: eventSourceFire
 *
 * Description:
 *         Invokes every registered listener's callback in registration order.
 *
 * Inputs:
 *         source : the EventSource whose listeners should be notified.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void eventSourceFire(EventSource *source)
{
    int i;

    for (i = 0; i < source->count; i++)
    {
        source->listeners[i].callback(source->listeners[i].userData);
    }
}

void onEventPrintString(void *userData)
{
    printf("listener got string: %s\n", (const char *)userData);
}

void onEventPrintInt(void *userData)
{
    printf("listener got int: %d\n", *(int *)userData);
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates a generic callback registration/event-firing pattern
 *         built on a void (*)(void *) callback type.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    EventSource source;
    int magicNumber = 42;

    eventSourceInit(&source);
    eventSourceRegister(&source, onEventPrintString, "hello event");
    eventSourceRegister(&source, onEventPrintInt, &magicNumber);

    eventSourceFire(&source);

    return 0;
}
