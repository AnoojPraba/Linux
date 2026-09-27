#include <iostream>

using namespace std;

// Classic "what's wrong with this code" interview snippet: deleting a
// derived object through a base class pointer when the base destructor is
// NOT virtual.

// --- The bug: no virtual destructor ---
class BrokenBase
{
    public:
        // Trivial constructor.
        BrokenBase() { cout << "BrokenBase constructed\n"; }

        // NOT virtual: deleting through a BrokenBase* will not know to call
        // any derived destructor.
        ~BrokenBase() { cout << "BrokenBase destroyed\n"; }
};

class BrokenDerived : public BrokenBase
{
    private:
        int *resource;

    public:
        /*****************************************************************************
         * Name: BrokenDerived
         *
         * Description:
         *         Allocates a heap resource so its destructor has visible
         *         cleanup work to (fail to) perform.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        BrokenDerived() : resource(new int(0))
        {
            cout << "BrokenDerived constructed\n";
        }

        /*****************************************************************************
         * Name: ~BrokenDerived
         *
         * Description:
         *         Releases the heap resource. With a non-virtual BrokenBase
         *         destructor, deleting a BrokenDerived through a BrokenBase*
         *         technically has undefined behavior, and in practice this
         *         destructor is observed to NOT run -- the resource "print"
         *         below never fires, meaning the cleanup was skipped.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        ~BrokenDerived()
        {
            cout << "BrokenDerived destroyed, freeing resource\n";
            delete resource;
        }
};

// --- The fix: virtual destructor ---
class FixedBase
{
    public:
        // Trivial constructor.
        FixedBase() { cout << "FixedBase constructed\n"; }

        // Virtual: deleting through a FixedBase* correctly runs the most
        // derived destructor first, then chains up to this one.
        virtual ~FixedBase() { cout << "FixedBase destroyed\n"; }
};

class FixedDerived : public FixedBase
{
    private:
        int *resource;

    public:
        // Trivial constructor.
        FixedDerived() : resource(new int(0))
        {
            cout << "FixedDerived constructed\n";
        }

        /*****************************************************************************
         * Name: ~FixedDerived
         *
         * Description:
         *         Releases the heap resource. Because FixedBase's destructor
         *         is virtual, deleting through a FixedBase* correctly invokes
         *         this destructor first.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        ~FixedDerived()
        {
            cout << "FixedDerived destroyed, freeing resource\n";
            delete resource;
        }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Deletes a derived object through a base pointer in both the broken
 *         (non-virtual destructor) and fixed (virtual destructor) cases,
 *         showing the derived destructor's cleanup being skipped versus
 *         correctly running.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    cout << "--- Broken: non-virtual base destructor ---\n";
    BrokenBase *broken = new BrokenDerived();
    delete broken;
    cout << "(BrokenDerived destructor above never printed -- its cleanup was skipped)\n\n";

    cout << "--- Fixed: virtual base destructor ---\n";
    FixedBase *fixed = new FixedDerived();
    delete fixed;
    return 0;
}
