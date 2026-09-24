#ifndef HANDLE_H
#define HANDLE_H

// The caller only ever sees a pointer to this incomplete type - its actual
// members are defined in handle.c, invisible here. This is the C
// equivalent of C++'s PIMPL idiom: callers can't access fields directly
// (there are none they can see), can't be broken by a field reordering in
// a future version (the ABI-compatibility question from
// Notes/26_SeniorCInterviewQuestions.c), and are forced through the
// accessor functions below, which can enforce invariants a raw struct
// field never could.
typedef struct Handle Handle;

Handle *handleCreate(int initialValue);
void handleDestroy(Handle *handle);
int handleGetValue(const Handle *handle);
void handleSetValue(Handle *handle, int value);

#endif
