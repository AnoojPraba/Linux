#include <stdio.h>

#define NUM_PROCESSES 5
#define NUM_RESOURCE_TYPES 3

// Banker's algorithm - a deadlock-avoidance technique (see NOTES.md and
// 17_DeadlockDetectionAvoidance for the broader context). Each process
// declares its maximum possible future claim on each resource type up
// front; the algorithm only grants an incremental request if doing so
// leaves the system in a "safe state" - one where there still exists some
// order in which every process could finish given its remaining max need.
int available[NUM_RESOURCE_TYPES] = { 3, 3, 2 };
int maxClaim[NUM_PROCESSES][NUM_RESOURCE_TYPES] =
{
    { 7, 5, 3 },
    { 3, 2, 2 },
    { 9, 0, 2 },
    { 2, 2, 2 },
    { 4, 3, 3 }
};
int allocation[NUM_PROCESSES][NUM_RESOURCE_TYPES] =
{
    { 0, 1, 0 },
    { 2, 0, 0 },
    { 3, 0, 2 },
    { 2, 1, 1 },
    { 0, 0, 2 }
};

/*****************************************************************************
 * Name: computeNeed
 *
 * Description:
 *         Computes each process's remaining need (maxClaim - allocation)
 *         for every resource type into the caller-supplied matrix.
 *
 * Inputs:
 *         need : output matrix of size [NUM_PROCESSES][NUM_RESOURCE_TYPES].
 *
 * Returns:
 *         None.
 *****************************************************************************/
void computeNeed(int need[NUM_PROCESSES][NUM_RESOURCE_TYPES])
{
    int p;
    int r;

    for (p = 0; p < NUM_PROCESSES; p++)
    {
        for (r = 0; r < NUM_RESOURCE_TYPES; r++)
        {
            need[p][r] = maxClaim[p][r] - allocation[p][r];
        }
    }
}

/*****************************************************************************
 * Name: findSafeSequence
 *
 * Description:
 *         Runs the Banker's algorithm safety check against the given
 *         available/allocation/need state: repeatedly finds an unfinished
 *         process whose need fits within the currently available
 *         resources, "runs" it to completion (releasing its allocation
 *         back to available), and records it in safeSequence. If every
 *         process can be finished this way, the state is safe.
 *
 * Inputs:
 *         workAvailable : available-resource vector to simulate against;
 *                         modified in place as processes are simulated.
 *         need          : each process's remaining need matrix.
 *         safeSequence  : output array of size NUM_PROCESSES receiving the
 *                         safe finishing order if one is found.
 *
 * Returns:
 *         Nonzero if a safe sequence was found (safeSequence is valid),
 *         zero if the state is unsafe.
 *****************************************************************************/
int findSafeSequence(int workAvailable[NUM_RESOURCE_TYPES],
                      int need[NUM_PROCESSES][NUM_RESOURCE_TYPES],
                      int safeSequence[NUM_PROCESSES])
{
    int finished[NUM_PROCESSES];
    int sequenceCount = 0;
    int p;
    int r;

    for (p = 0; p < NUM_PROCESSES; p++)
    {
        finished[p] = 0;
    }

    while (sequenceCount < NUM_PROCESSES)
    {
        int foundCandidate = 0;

        for (p = 0; p < NUM_PROCESSES; p++)
        {
            int fits;

            if (finished[p])
            {
                continue;
            }

            fits = 1;
            for (r = 0; r < NUM_RESOURCE_TYPES; r++)
            {
                if (need[p][r] > workAvailable[r])
                {
                    fits = 0;
                    break;
                }
            }

            if (fits)
            {
                for (r = 0; r < NUM_RESOURCE_TYPES; r++)
                {
                    workAvailable[r] += allocation[p][r];
                }
                finished[p] = 1;
                safeSequence[sequenceCount] = p;
                sequenceCount++;
                foundCandidate = 1;
            }
        }

        if (!foundCandidate)
        {
            return 0;
        }
    }

    return 1;
}

/*****************************************************************************
 * Name: isSystemInSafeState
 *
 * Description:
 *         Convenience wrapper that copies available into a scratch vector,
 *         runs findSafeSequence(), and prints the resulting safe sequence
 *         if one exists.
 *
 * Returns:
 *         Nonzero if the current global state is safe, zero otherwise.
 *****************************************************************************/
int isSystemInSafeState(void)
{
    int need[NUM_PROCESSES][NUM_RESOURCE_TYPES];
    int workAvailable[NUM_RESOURCE_TYPES];
    int safeSequence[NUM_PROCESSES];
    int r;
    int i;

    computeNeed(need);
    for (r = 0; r < NUM_RESOURCE_TYPES; r++)
    {
        workAvailable[r] = available[r];
    }

    if (!findSafeSequence(workAvailable, need, safeSequence))
    {
        return 0;
    }

    printf("safe sequence: ");
    for (i = 0; i < NUM_PROCESSES; i++)
    {
        printf("P%d ", safeSequence[i]);
    }
    printf("\n");
    return 1;
}

/*****************************************************************************
 * Name: requestResources
 *
 * Description:
 *         Attempts to grant an incremental resource request from
 *         processId: tentatively applies it (reducing available,
 *         increasing that process's allocation), re-runs the safety check,
 *         and rolls the tentative grant back if the resulting state would
 *         be unsafe. Also rejects requests that exceed the process's
 *         declared remaining need or the currently available resources.
 *
 * Inputs:
 *         processId : index of the requesting process.
 *         request   : requested amount of each resource type.
 *
 * Returns:
 *         Nonzero if the request was granted, zero if denied.
 *****************************************************************************/
int requestResources(int processId, int request[NUM_RESOURCE_TYPES])
{
    int need[NUM_PROCESSES][NUM_RESOURCE_TYPES];
    int workAvailable[NUM_RESOURCE_TYPES];
    int safeSequence[NUM_PROCESSES];
    int r;

    computeNeed(need);

    for (r = 0; r < NUM_RESOURCE_TYPES; r++)
    {
        if ((request[r] > need[processId][r]) || (request[r] > available[r]))
        {
            printf("P%d request denied: exceeds declared need or availability\n",
                   processId);
            return 0;
        }
    }

    for (r = 0; r < NUM_RESOURCE_TYPES; r++)
    {
        available[r] -= request[r];
        allocation[processId][r] += request[r];
    }

    computeNeed(need);
    for (r = 0; r < NUM_RESOURCE_TYPES; r++)
    {
        workAvailable[r] = available[r];
    }

    if (findSafeSequence(workAvailable, need, safeSequence))
    {
        printf("P%d request granted: resulting state is safe\n", processId);
        return 1;
    }

    for (r = 0; r < NUM_RESOURCE_TYPES; r++)
    {
        available[r] += request[r];
        allocation[processId][r] -= request[r];
    }
    printf("P%d request denied: would leave the system in an unsafe state\n",
           processId);
    return 0;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates the Banker's algorithm: verifies the initial state
 *         is safe, grants a request that keeps the state safe, then
 *         demonstrates a request that is denied because it would create an
 *         unsafe state.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    int safeRequest[NUM_RESOURCE_TYPES] = { 1, 0, 2 };
    int unsafeRequest[NUM_RESOURCE_TYPES] = { 3, 3, 0 };

    printf("initial state:\n");
    isSystemInSafeState();

    printf("\nP1 requests (1, 0, 2) - within need and available:\n");
    requestResources(1, safeRequest);
    isSystemInSafeState();

    printf("\nP4 requests (3, 3, 0) - exceeds current available/would be unsafe:\n");
    requestResources(4, unsafeRequest);

    return 0;
}
