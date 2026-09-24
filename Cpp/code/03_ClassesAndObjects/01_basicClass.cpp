#include <iostream>
#include <string>

using namespace std;

#define MAX_HEALTH 100

// Demonstrates a basic class with private data, public member functions,
// and access specifiers (private/protected/public).
class Character
{
    private:
        string name;
        int health;

    public:
        // Trivial constructor with initializer list.
        Character(const string &charName, int startHealth)
            : name(charName), health(startHealth)
        {
        }

        // Trivial getter.
        int getHealth() const { return health; }

        /*****************************************************************************
         * Name: takeDamage
         *
         * Description:
         *         Reduces the character's health by the given amount, clamped at zero.
         *
         * Inputs:
         *         amount : the amount of damage to apply.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void takeDamage(int amount)
        {
            health -= amount;
            if (health < 0)
            {
                health = 0;
            }
        }

        /*****************************************************************************
         * Name: describe
         *
         * Description:
         *         Prints the character's current name and health to stdout.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void describe() const
        {
            cout << name << " has " << health << "/" << MAX_HEALTH << " health\n";
        }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Creates a Character object, exercises its member functions, and
 *         prints the results.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    Character hero("Aria", MAX_HEALTH);
    hero.describe();
    hero.takeDamage(30);
    hero.describe();
    return 0;
}
