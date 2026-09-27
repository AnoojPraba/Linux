#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

// Contrast with 01_ruleOfThree.cpp: Buffer there owns a raw resource directly
// (a char*), so it MUST define its own destructor, copy constructor, and copy
// assignment operator (rule of three). PlayerRecord below owns NOTHING raw --
// every member is already an RAII type (std::string, std::vector,
// std::unique_ptr) that correctly manages its own destruction, copying, and
// moving. Because of that, PlayerRecord needs NO custom destructor/copy/move
// operations at all: the compiler-generated ones just call each member's own
// special member function, which is already correct. This "rule of zero" is
// the modern preferred default -- rule of three/five only kicks in once a
// class directly owns a raw resource (a pointer obtained via new/malloc, a
// file handle, etc.) that the RAII wrapper types don't already cover.
//
// Note: because one member is a std::unique_ptr (move-only by design), the
// compiler-generated copy constructor/assignment are themselves implicitly
// deleted -- rule of zero still applies correctly here, it just faithfully
// makes PlayerRecord move-only without any hand-written code, and the
// compiler-generated move constructor/assignment work fine.
class PlayerRecord
{
    private:
        string name;
        vector<int> scores;
        unique_ptr<int> bonusRound;

    public:
        // Trivial constructor with initializer list.
        PlayerRecord(string playerName, vector<int> playerScores)
            : name(std::move(playerName)), scores(std::move(playerScores)),
              bonusRound(make_unique<int>(0))
        {
        }

        // No destructor, copy, or move members declared here -- the
        // compiler-generated ones are correct and sufficient (rule of zero).

        /*****************************************************************************
         * Name: describe
         *
         * Description:
         *         Prints the player's name and score count.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void describe() const
        {
            cout << name << " has " << scores.size() << " recorded scores\n";
        }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Creates a PlayerRecord and move-constructs another from it, to show
 *         the compiler-generated special member functions work correctly
 *         with no custom code.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    PlayerRecord original("Nova", {10, 20, 30});
    PlayerRecord moved = std::move(original);

    moved.describe();
    return 0;
}
