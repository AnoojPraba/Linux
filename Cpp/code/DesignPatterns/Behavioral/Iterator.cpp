#include <iostream>
#include <string>
#include <vector>

// Iterator: traverse a playlist without exposing its internal storage.
class Playlist
{
public:
    /*****************************************************************************
     * Name: addSong
     *
     * Description:
     *         Appends a song title to the playlist.
     *
     * Inputs:
     *         title : the song title to add.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void addSong(const std::string& title)
    {
        songs.push_back(title);
    }

    class Iterator
    {
    public:
        // trivial constructor
        Iterator(const std::vector<std::string>& songs, size_t index)
            : songs(songs), index(index) {}

        // trivial
        bool operator!=(const Iterator& other) const
        {
            return index != other.index;
        }

        // trivial
        void operator++()
        {
            ++index;
        }

        // trivial
        const std::string& operator*() const
        {
            return songs[index];
        }

    private:
        const std::vector<std::string>& songs;
        size_t index;
    };

    // trivial
    Iterator begin() const
    {
        return Iterator(songs, 0);
    }

    // trivial
    Iterator end() const
    {
        return Iterator(songs, songs.size());
    }

private:
    std::vector<std::string> songs;
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Builds a playlist and iterates over it using the custom Iterator,
 *         demonstrating support for range-based for loops.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    Playlist playlist;
    playlist.addSong("Song A");
    playlist.addSong("Song B");
    playlist.addSong("Song C");

    for (const auto& song : playlist)
    {
        std::cout << "Now playing: " << song << std::endl;
    }
    return 0;
}
