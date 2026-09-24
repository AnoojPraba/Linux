#include <iostream>
#include <memory>

using namespace std;

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
    explicit RealImage(const string& filename) : filename(filename)
    {
        cout << "Loading image from disk: " << filename << endl;
    }

    // trivial override
    void display() const override
    {
        cout << "Displaying " << filename << endl;
    }

private:
    string filename;
};

// Proxy that defers construction of the RealImage until display() is called.
class ImageProxy : public Image
{
public:
    // trivial constructor
    explicit ImageProxy(const string& filename) : filename(filename) {}

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
            realImage = make_unique<RealImage>(filename);
        }
        realImage->display();
    }

private:
    string filename;
    mutable unique_ptr<RealImage> realImage;
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
    cout << "Proxy created, image not yet loaded." << endl;
    proxy.display();
    proxy.display();
    return 0;
}
