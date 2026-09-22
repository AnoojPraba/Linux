#include <iostream>
#include <memory>
#include <vector>

#define INDENT_SPACES 2

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
    explicit File(const std::string& name) : name(name) {}

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
        std::cout << std::string(depth * INDENT_SPACES, ' ') << "File: " << name << std::endl;
    }

private:
    std::string name;
};

class Folder : public FileSystemItem
{
public:
    // trivial constructor
    explicit Folder(const std::string& name) : name(name) {}

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
    void add(std::unique_ptr<FileSystemItem> item)
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
        std::cout << std::string(depth * INDENT_SPACES, ' ') << "Folder: " << name
                  << std::endl;
        for (const auto& child : children)
        {
            child->print(depth + 1);
        }
    }

private:
    std::string name;
    std::vector<std::unique_ptr<FileSystemItem>> children;
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
    auto root = std::make_unique<Folder>("root");
    root->add(std::make_unique<File>("readme.txt"));

    auto docs = std::make_unique<Folder>("docs");
    docs->add(std::make_unique<File>("design.md"));
    root->add(std::move(docs));

    root->print(0);
    return 0;
}
