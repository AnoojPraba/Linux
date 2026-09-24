#include <iostream>
#include <memory>
#include <vector>

#define INDENT_SPACES 2

using namespace std;

// Composite: files and folders share a uniform interface.
class FileSystemItem
{
public:
    virtual ~FileSystemItem() = default;
    virtual void print(int depth) const = 0;
};

class File : public FileSystemItem
{
public:
    // trivial constructor
    explicit File(const string& name) : name(name) {}

    /*****************************************************************************
     * Name: print
     *
     * Description:
     *         Prints the file name indented according to its depth in the tree.
     *
     * Inputs:
     *         depth : how many indent levels to print before the name.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void print(int depth) const override
    {
        cout << string(depth * INDENT_SPACES, ' ') << "File: " << name << endl;
    }

private:
    string name;
};

class Folder : public FileSystemItem
{
public:
    // trivial constructor
    explicit Folder(const string& name) : name(name) {}

    /*****************************************************************************
     * Name: add
     *
     * Description:
     *         Adds a child item (file or folder) to this folder.
     *
     * Inputs:
     *         item : the item to add as a child.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void add(unique_ptr<FileSystemItem> item)
    {
        children.push_back(std::move(item));
    }

    /*****************************************************************************
     * Name: print
     *
     * Description:
     *         Prints the folder name and then recursively prints every child.
     *
     * Inputs:
     *         depth : how many indent levels to print before the name.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void print(int depth) const override
    {
        cout << string(depth * INDENT_SPACES, ' ') << "Folder: " << name
                  << endl;
        for (const auto& child : children)
        {
            child->print(depth + 1);
        }
    }

private:
    string name;
    vector<unique_ptr<FileSystemItem>> children;
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Builds a small folder/file tree and prints it recursively through
 *         the common FileSystemItem interface.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    auto root = make_unique<Folder>("root");
    root->add(make_unique<File>("readme.txt"));

    auto docs = make_unique<Folder>("docs");
    docs->add(make_unique<File>("design.md"));
    root->add(std::move(docs));

    root->print(0);
    return 0;
}
