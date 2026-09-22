#include <iostream>
#include <cstdio>
#include <string>

// Demonstrates RAII via a class that manages a FILE* resource: the file is
// opened in the constructor and always closed in the destructor.
class FileGuard
{
    private:
        FILE *file;

    public:
        /*****************************************************************************
         * Name: FileGuard
         *
         * Description:
         *         Opens the given file path with the given mode, acquiring
         *         ownership of the underlying FILE resource.
         *
         * Inputs:
         *         path : filesystem path to open.
         *         mode : fopen-style mode string (e.g. "w").
         *
         * Returns:
         *         None.
         *****************************************************************************/
        FileGuard(const std::string &path, const std::string &mode)
        {
            file = std::fopen(path.c_str(), mode.c_str());
        }

        /*****************************************************************************
         * Name: ~FileGuard
         *
         * Description:
         *         Releases the owned FILE resource, if any, ensuring it is closed
         *         regardless of how the enclosing scope is exited.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        ~FileGuard()
        {
            if (file != nullptr)
            {
                std::fclose(file);
                std::cout << "FileGuard closed the file\n";
            }
        }

        FileGuard(const FileGuard &) = delete;
        FileGuard &operator=(const FileGuard &) = delete;

        /*****************************************************************************
         * Name: isOpen
         *
         * Description:
         *         Reports whether the underlying file was opened successfully.
         *
         * Returns:
         *         true if open, false otherwise.
         *****************************************************************************/
        bool isOpen() const
        {
            return file != nullptr;
        }

        /*****************************************************************************
         * Name: write
         *
         * Description:
         *         Writes a line of text to the managed file.
         *
         * Inputs:
         *         text : the text to write.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void write(const std::string &text)
        {
            if (file != nullptr)
            {
                std::fputs(text.c_str(), file);
            }
        }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Opens a temporary file inside a scope via FileGuard, writes to it,
 *         and lets the destructor close it automatically when the scope ends.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    {
        FileGuard guard("/tmp/raii_example.txt", "w");
        if (guard.isOpen())
        {
            guard.write("RAII manages this file's lifetime\n");
            std::cout << "wrote to file inside scope\n";
        }
    }
    std::cout << "file was closed automatically when the scope ended\n";
    return 0;
}
