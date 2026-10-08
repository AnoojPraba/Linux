# 06_FriendFunctionsAndClasses

friend functions and friend classes granting access to private members.

## Files
- `01_friends.cpp` - Box with a private constructor and factory; a friend function and a friend class reading its private members

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread 01_friends.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `..` (the parent code/ dir) builds everything into `Cpp/code/bin/06_FriendFunctionsAndClasses/` (git-ignored).

## Key concepts / interview angles
- Friendship is not inherited, not transitive and not mutual.
- Typical legitimate uses: `operator<<` and symmetric binary operators, tightly coupled helpers, factories with private constructors.
- Friends weaken encapsulation; prefer public interfaces when possible.

## Related
- `../07_OperatorOverloading`
- `../28_PImplIdiom`
- `../03_ClassesAndObjects`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
