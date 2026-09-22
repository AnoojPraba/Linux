#include <iostream>
#include <memory>

// Proxy: a lazily-loaded, access-controlled stand-in for a large image.
class Image
{
public:
    virtual ~Image() = default;
    virtual void display() const = 0;
};

class RealImage : public Image
{
public:
    /*****************************************************************************
     * Name: RealImage
     *
     * Description:
     *         Loads the image data from disk, an expensive operation we want
     *         to defer until it is truly needed.
     *
     * Inputs:
     *         filename : path of the image file to load.
     *
     *****************************************************************************/
    explicit RealImage(const std::string& filename) : filename(filename)
    {
        std::cout << "Loading image from disk: " << filename << std::endl;
    }

    // trivial override
    void display() const override
    {
        std::cout << "Displaying " << filename << std::endl;
    }

private:
    std::string filename;
};

// Proxy that defers construction of the RealImage until display() is called.
class ImageProxy : public Image
{
public:
    // trivial constructor
    explicit ImageProxy(const std::string& filename) : filename(filename) {}

    /*****************************************************************************
     * Name: display
     *
     * Description:
     *         Lazily creates the real image on first use and forwards the
     *         display request to it.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void display() const override
    {
        if (!realImage)
        {
            realImage = std::make_unique<RealImage>(filename);
        }
        realImage->display();
    }

private:
    std::string filename;
    mutable std::unique_ptr<RealImage> realImage;
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Shows that the real image is only loaded the first time it is
 *         displayed through the proxy.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    ImageProxy proxy("landscape.png");
    std::cout << "Proxy created, image not yet loaded." << std::endl;
    proxy.display();
    proxy.display();
    return 0;
}
