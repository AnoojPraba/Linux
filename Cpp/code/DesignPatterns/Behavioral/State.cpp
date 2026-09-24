#include <iostream>
#include <memory>

using namespace std;

// State: a media player's behavior changes with its playback state.
class PlayerState
{
public:
    virtual ~PlayerState() = default;
    virtual unique_ptr<PlayerState> pressPlay() const = 0;
    virtual string name() const = 0;
};

class PlayingState : public PlayerState
{
public:
    unique_ptr<PlayerState> pressPlay() const override;

    // trivial override
    string name() const override
    {
        return "Playing";
    }
};

class PausedState : public PlayerState
{
public:
    /*****************************************************************************
     * Name: pressPlay
     *
     * Description:
     *         Transitions from paused back to playing when play is pressed.
     *
     * Returns:
     *         A new PlayingState representing the next state.
     *****************************************************************************/
    unique_ptr<PlayerState> pressPlay() const override
    {
        cout << "Resuming playback" << endl;
        return make_unique<PlayingState>();
    }

    // trivial override
    string name() const override
    {
        return "Paused";
    }
};

/*****************************************************************************
 * Name: PlayingState::pressPlay
 *
 * Description:
 *         Transitions from playing to paused when play is pressed again.
 *
 * Returns:
 *         A new PausedState representing the next state.
 *****************************************************************************/
unique_ptr<PlayerState> PlayingState::pressPlay() const
{
    cout << "Pausing playback" << endl;
    return make_unique<PausedState>();
}

class MediaPlayer
{
public:
    // trivial constructor
    MediaPlayer() : state(make_unique<PausedState>()) {}

    /*****************************************************************************
     * Name: pressPlay
     *
     * Description:
     *         Delegates the play button press to the current state and stores
     *         whatever state it transitions to.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void pressPlay()
    {
        state = state->pressPlay();
        cout << "Now in state: " << state->name() << endl;
    }

private:
    unique_ptr<PlayerState> state;
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Presses play on a media player repeatedly to show it toggling
 *         between the playing and paused states.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    MediaPlayer player;
    player.pressPlay();
    player.pressPlay();
    player.pressPlay();
    return 0;
}
