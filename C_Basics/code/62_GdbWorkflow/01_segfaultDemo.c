#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Deliberately buggy: buildRecord() returns a pointer into a local buffer
// that is never actually null in this simplified version, but
// summarizeRecord() is called with a NULL record whenever the name lookup
// "fails" (index out of range), and then dereferences it. Compiles cleanly;
// crashes at runtime when run with an out-of-range index - which is the
// point of the NOTES.md gdb walkthrough alongside this file.
#define MAX_RECORDS 3
#define NAME_BUF_LEN 32

typedef struct
{
    int id;
    char name[NAME_BUF_LEN];
} Record;

/*****************************************************************************
 * Name: findRecord
 *
 * Description:
 *         Looks up a record by index in a fixed table, returning NULL when
 *         the index is out of range instead of aborting.
 *
 * Inputs:
 *         records : the table of records to search.
 *         index   : the index to look up.
 *
 * Returns:
 *         Pointer to the matching record, or NULL if index is out of range.
 *****************************************************************************/
Record *findRecord(Record *records, int index)
{
    if ((index < 0) || (index >= MAX_RECORDS))
    {
        return NULL;
    }
    return &records[index];
}

/*****************************************************************************
 * Name: summarizeRecord
 *
 * Description:
 *         Prints a record's id and name. Does not check for NULL - the bug
 *         under investigation - so a caller passing an out-of-range lookup
 *         result crashes here with a null-pointer dereference.
 *
 * Inputs:
 *         record : the record to summarize.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void summarizeRecord(Record *record)
{
    printf("record %d: %s\n", record->id, record->name);
}

int main(int argc, char *argv[])
{
    Record records[MAX_RECORDS];
    int i;
    int lookupIndex;
    Record *found;

    for (i = 0; i < MAX_RECORDS; i++)
    {
        records[i].id = i;
        snprintf(records[i].name, NAME_BUF_LEN, "record-%d", i);
    }

    // Passing an index of 5 on the command line reproduces the crash;
    // with no args, lookupIndex defaults to 1 (in range) and it runs fine.
    lookupIndex = (argc > 1) ? atoi(argv[1]) : 1;

    found = findRecord(records, lookupIndex);
    summarizeRecord(found);

    return 0;
}
