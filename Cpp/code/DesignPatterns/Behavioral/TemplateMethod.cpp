#include <iostream>

using namespace std;

// Template Method: fixed document export skeleton, customizable per format.
class DocumentExporter
{
public:
    virtual ~DocumentExporter() = default;

    /*****************************************************************************
     * Name: exportDocument
     *
     * Description:
     *         Runs the fixed export skeleton: open, write the body, then
     *         close, delegating the format-specific parts to subclasses.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void exportDocument() const
    {
        openFile();
        writeBody();
        closeFile();
    }

protected:
    // shared step, same for every format
    void openFile() const
    {
        cout << "Opening output file" << endl;
    }

    virtual void writeBody() const = 0;

    // shared step, same for every format
    void closeFile() const
    {
        cout << "Closing output file" << endl;
    }
};

class PdfExporter : public DocumentExporter
{
protected:
    // trivial override
    void writeBody() const override
    {
        cout << "Writing body as PDF content" << endl;
    }
};

class HtmlExporter : public DocumentExporter
{
protected:
    // trivial override
    void writeBody() const override
    {
        cout << "Writing body as HTML content" << endl;
    }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Exports a document using both the PDF and HTML exporters to show
 *         the shared skeleton with different body formatting.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    PdfExporter pdf;
    HtmlExporter html;
    pdf.exportDocument();
    html.exportDocument();
    return 0;
}
