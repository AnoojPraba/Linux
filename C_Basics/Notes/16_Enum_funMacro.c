// Fun macro + function for testing output utilizing the STRINGSIZE OPERATOR
// ============================================================================
#define DEFINE_CASE_FOR_STRING(x) \
    case (x):                     \
        return #x;
// Print out test state name in console for debugging
char *GetName(initTestState_t state)
{
    switch (state) {
        DEFINE_CASE_FOR_STRING(state1);
        DEFINE_CASE_FOR_STRING(state2);
        DEFINE_CASE_FOR_STRING(state3);
    }
    return "UNKNOWN";
}

