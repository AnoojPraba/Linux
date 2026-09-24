#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

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
    const string path = "/tmp/streams_example.txt";

    ofstream outFile(path);
    if (outFile.is_open())
    {
        outFile << "Alice 30\nBob 25\n";
        outFile.close();
    }

    ifstream inFile(path);
    string line;
    cout << "reading file back:\n";
    while (getline(inFile, line))
    {
        cout << "  " << line << "\n";
    }
    inFile.close();

    stringstream ss("Charlie 40");
    string name;
    int age;
    ss >> name >> age;
    cout << "parsed from stringstream: name=" << name << ", age=" << age << "\n";
    return 0;
}
