// Bit flags in modern C++: enum class with operator overloads, bit-fields,
// packing fields into an integer and flag sets with std::bitset.

#include <bitset>
#include <cstdint>
#include <iostream>
#include <type_traits>

enum class Perm : unsigned { None = 0, Read = 1, Write = 2, Exec = 4 };

constexpr unsigned raw(Perm p) { return static_cast<std::underlying_type_t<Perm>>(p); }
constexpr Perm operator|(Perm a, Perm b) { return static_cast<Perm>(raw(a) | raw(b)); }
constexpr Perm operator&(Perm a, Perm b) { return static_cast<Perm>(raw(a) & raw(b)); }
constexpr Perm operator~(Perm a) { return static_cast<Perm>(~raw(a) & 7u); }
constexpr Perm &operator|=(Perm &a, Perm b) { return a = a | b; }
constexpr Perm &operator&=(Perm &a, Perm b) { return a = a & b; }
constexpr bool has(Perm set, Perm flag) { return (set & flag) == flag; }

std::string text(Perm p)
{
    return std::string(has(p, Perm::Read) ? "r" : "-") + (has(p, Perm::Write) ? "w" : "-") + (has(p, Perm::Exec) ? "x" : "-");
}

struct Packed                                                        // bit-fields: layout and order are implementation-defined
{
    unsigned year : 7;                                               // years since 1980
    unsigned month : 4;
    unsigned day : 5;
};

constexpr std::uint16_t pack_date(int year, int month, int day) { return static_cast<std::uint16_t>((year - 1980) << 9 | month << 5 | day); }

int main()
{
    Perm p = Perm::Read | Perm::Write;
    std::cout << "owner " << text(p) << '\n';
    p |= Perm::Exec;
    std::cout << "+x    " << text(p) << '\n';
    p &= ~Perm::Write;
    std::cout << "-w    " << text(p) << ", has Exec: " << std::boolalpha << has(p, Perm::Exec) << ", has Write: " << has(p, Perm::Write) << '\n';
    std::cout << "(an unscoped enum would convert to int silently; enum class needs the operators above)\n";

    std::uint16_t d = pack_date(2026, 10, 12);
    std::cout << "packed date 0x" << std::hex << d << std::dec << " -> year " << (d >> 9) + 1980 << " month " << (d >> 5 & 15) << " day " << (d & 31) << '\n';
    Packed bf{46, 10, 12};
    std::cout << "bit-field struct holds the same date in sizeof = " << sizeof(Packed) << " bytes: " << bf.year + 1980 << '-' << bf.month << '-' << bf.day << '\n';

    std::bitset<8> features;
    enum Feature { Wifi, Bluetooth, Gps, Nfc };
    features.set(Wifi).set(Gps);
    std::cout << "feature flags " << features << ", GPS: " << features.test(Gps) << ", NFC: " << features.test(Nfc) << '\n';
}
