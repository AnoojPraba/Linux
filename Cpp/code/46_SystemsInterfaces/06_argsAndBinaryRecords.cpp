// argv via span/string_view, binary records with seekg/seekp random access,
// trivially-copyable static_assert.

#include <cstdio>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

struct Record
{
    std::uint32_t id;
    double score;
};
static_assert(std::is_trivially_copyable_v<Record>, "raw byte I/O needs a trivially copyable type");

int run(std::span<char *> args)                         // args[0] is the program name
{
    std::vector<std::string_view> opts(args.begin() + 1, args.end());
    int repeat = 1;
    for (std::size_t i = 0; i < opts.size(); ++i)
        if (opts[i] == "--repeat" && i + 1 < opts.size())
            repeat = std::stoi(std::string(opts[++i]));
    std::cout << "options: " << opts.size() << ", repeat = " << repeat << '\n';

    const char *path = "records_demo2.bin";
    {
        std::ofstream out(path, std::ios::binary);
        for (std::uint32_t i = 1; i <= 5; ++i)
        {
            Record r{i, i * 2.5};
            out.write(reinterpret_cast<const char *>(&r), sizeof r);
        }
    }

    std::fstream f(path, std::ios::binary | std::ios::in | std::ios::out);
    f.seekg(0, std::ios::end);
    std::cout << "records in file: " << f.tellg() / static_cast<std::streamoff>(sizeof(Record)) << '\n';

    Record r{};
    f.seekg(3 * static_cast<std::streamoff>(sizeof r));  // random access: jump straight to record 3
    f.read(reinterpret_cast<char *>(&r), sizeof r);
    std::cout << "record 3: id=" << r.id << " score=" << r.score << '\n';

    r.score = 99.5;
    f.seekp(3 * static_cast<std::streamoff>(sizeof r));  // overwrite it in place
    f.write(reinterpret_cast<const char *>(&r), sizeof r);
    f.flush();
    f.seekg(3 * static_cast<std::streamoff>(sizeof r));
    f.read(reinterpret_cast<char *>(&r), sizeof r);
    std::cout << "after update: score=" << r.score << '\n';
    f.close();
    std::remove(path);
    return 0;
}

int main(int argc, char *argv[])
{
    return run(std::span<char *>(argv, static_cast<std::size_t>(argc)));
}
