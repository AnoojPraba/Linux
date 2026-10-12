// Constructor/destructor order, slicing and the virtual-base diamond.

#include <iostream>
#include <string>

struct Animal
{
    std::string name;
    explicit Animal(std::string n) : name(std::move(n)) { std::cout << "Animal(" << name << ") "; }
    virtual ~Animal() { std::cout << "~Animal "; }
    virtual std::string speak() const { return "..."; }
};

struct Mammal : Animal                                  // multilevel: Animal <- Mammal <- Dog
{
    explicit Mammal(const std::string &n) : Animal(n) { std::cout << "Mammal "; }
    ~Mammal() override { std::cout << "~Mammal "; }
};

struct Dog : Mammal
{
    int tricks = 3;
    explicit Dog(const std::string &n) : Mammal(n) { std::cout << "Dog "; }
    ~Dog() override { std::cout << "~Dog "; }
    std::string speak() const override { return "woof"; }
};

// The diamond: two paths from Pet to Base. Without 'virtual' there would be two Base sub-objects.
struct Base { int id = 0; Base() { std::cout << "Base "; } virtual ~Base() = default; };
struct Left : virtual Base { Left() { std::cout << "Left "; } };
struct Right : virtual Base { Right() { std::cout << "Right "; } };
struct Pet : Left, Right { Pet() { std::cout << "Pet "; } };

std::string by_value(Animal a) { return a.speak(); }    // takes an Animal by value: the Dog part is sliced off

int main()
{
    std::cout << "construction: ";
    {
        Dog d("Rex");
        std::string sliced = by_value(d);                // the Animal copy is destroyed here: "~Animal" is printed
        std::cout << "\nthrough Animal&: " << static_cast<Animal &>(d).speak() << ", sliced copy says: " << sliced << '\n';
        std::cout << "destruction: ";
    }
    std::cout << '\n';

    std::cout << "diamond: ";
    Pet p;
    p.id = 5;                                            // unambiguous: there is exactly one Base
    std::cout << "\nsizeof(Pet) = " << sizeof(Pet) << ", one shared Base id = " << static_cast<Left &>(p).id << '\n';
}
