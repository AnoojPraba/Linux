#include <iostream>
#include <string>

// Demonstrates single inheritance, protected members, and constructor
// chaining via a base class initializer list call.
class Animal
{
    protected:
        std::string name;

    public:
        // Trivial constructor with initializer list.
        Animal(const std::string &animalName) : name(animalName) {}

        /*****************************************************************************
         * Name: speak
         *
         * Description:
         *         Prints a generic animal sound message using the protected name.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void speak() const
        {
            std::cout << name << " makes a sound\n";
        }
};

class Dog : public Animal
{
    private:
        std::string breed;

    public:
        /*****************************************************************************
         * Name: Dog
         *
         * Description:
         *         Constructs a Dog, chaining to the Animal base constructor to
         *         initialize the shared name field.
         *
         * Inputs:
         *         dogName  : the dog's name, forwarded to the Animal base class.
         *         dogBreed : the dog's breed.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        Dog(const std::string &dogName, const std::string &dogBreed)
            : Animal(dogName), breed(dogBreed)
        {
        }

        /*****************************************************************************
         * Name: bark
         *
         * Description:
         *         Prints a bark message using the base class's protected name field.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void bark() const
        {
            std::cout << name << " the " << breed << " says Woof!\n";
        }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Creates a Dog instance and exercises both inherited and derived
 *         member functions.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    Dog rex("Rex", "Labrador");
    rex.speak();
    rex.bark();
    return 0;
}
