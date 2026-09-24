#include <iostream>

using namespace std;

// Facade: a simple interface hiding a home theater's complex subsystems.
class Amplifier
{
public:
    // trivial
    void on() const
    {
        cout << "Amplifier on" << endl;
    }
};

class DvdPlayer
{
public:
    /*****************************************************************************
     * Name: play
     *
     * Description:
     *         Starts playback of the given movie.
     *
     * Inputs:
     *         movie : the title to play.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void play(const string& movie) const
    {
        cout << "Playing movie: " << movie << endl;
    }
};

class Projector
{
public:
    // trivial
    void on() const
    {
        cout << "Projector on" << endl;
    }
};

// Facade that coordinates the subsystems for the common "watch a movie" case.
class HomeTheaterFacade
{
public:
    /*****************************************************************************
     * Name: watchMovie
     *
     * Description:
     *         Powers on the amplifier and projector, then starts the movie,
     *         hiding the subsystem details from the caller.
     *
     * Inputs:
     *         movie : the title to watch.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void watchMovie(const string& movie) const
    {
        amplifier.on();
        projector.on();
        dvdPlayer.play(movie);
    }

private:
    Amplifier amplifier;
    Projector projector;
    DvdPlayer dvdPlayer;
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Uses the facade to watch a movie without touching the subsystems
 *         directly.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    HomeTheaterFacade homeTheater;
    homeTheater.watchMovie("The Matrix");
    return 0;
}
