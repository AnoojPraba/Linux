#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Memento: an opaque snapshot of the originator's state.
class EditorMemento
{
public:
    // ctor stores the snapshot
    explicit EditorMemento(string textIn) : text(std::move(textIn)) {}

    // trivial getter, only meant to be used by the originator that created it
    const string& getText() const
    {
        return text;
    }

private:
    string text;
};

// Originator: produces mementos of its state and can restore from one.
class TextEditor
{
public:
    /*****************************************************************************
     * Name: type
     *
     * Description:
     *         Appends text to the document being edited.
     *
     * Inputs:
     *         text : the text to append.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void type(const string& text)
    {
        content += text;
    }

    // trivial getter
    const string& getContent() const
    {
        return content;
    }

    /*****************************************************************************
     * Name: save
     *
     * Description:
     *         Captures the editor's current content as a memento.
     *
     * Returns:
     *         An EditorMemento snapshot of the current content.
     *****************************************************************************/
    EditorMemento save() const
    {
        return EditorMemento(content);
    }

    /*****************************************************************************
     * Name: restore
     *
     * Description:
     *         Replaces the editor's content with the content captured in a
     *         previously saved memento.
     *
     * Inputs:
     *         memento : the snapshot to restore from.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void restore(const EditorMemento& memento)
    {
        content = memento.getText();
    }

private:
    string content;
};

// Caretaker: holds the history of mementos without inspecting their contents.
class History
{
public:
    // trivial push
    void push(const EditorMemento& memento)
    {
        snapshots.push_back(memento);
    }

    /*****************************************************************************
     * Name: pop
     *
     * Description:
     *         Removes and returns the most recently saved memento, for
     *         undo-style restoration.
     *
     * Returns:
     *         The most recent EditorMemento snapshot.
     *****************************************************************************/
    EditorMemento pop()
    {
        EditorMemento last = snapshots.back();
        snapshots.pop_back();
        return last;
    }

private:
    vector<EditorMemento> snapshots;
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Types text into an editor while saving snapshots after each step,
 *         then undoes changes by restoring from the history's mementos.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    TextEditor editor;
    History history;

    editor.type("Hello");
    history.push(editor.save());

    editor.type(", world");
    history.push(editor.save());

    editor.type("!!!");
    cout << "Current: " << editor.getContent() << endl;

    editor.restore(history.pop());
    cout << "After undo 1: " << editor.getContent() << endl;

    editor.restore(history.pop());
    cout << "After undo 2: " << editor.getContent() << endl;
    return 0;
}
