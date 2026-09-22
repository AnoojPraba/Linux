#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates ofstream, ifstream, and stringstream usage: writes a
 *         file, reads it back, and parses fields from a stringstream.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    const std::string path = "/tmp/streams_example.txt";

    std::ofstream outFile(path);
    if (outFile.is_open())
    {
        outFile << "Alice 30\nBob 25\n";
        outFile.close();
    }

    std::ifstream inFile(path);
    std::string line;
    std::cout << "reading file back:\n";
    while (std::getline(inFile, line))
    {
        std::cout << "  " << line << "\n";
    }
    inFile.close();

    std::stringstream ss("Charlie 40");
    std::string name;
    int age;
    ss >> name >> age;
    std::cout << "parsed from stringstream: name=" << name << ", age=" << age << "\n";
    return 0;
}
