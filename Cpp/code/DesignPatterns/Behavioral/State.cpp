#include <iostream>
#include <memory>

// State: a media player's behavior changes with its playback state.
class PlayerState
{
public:
    virtual ~PlayerState() = default;
    virtual std::unique_ptr<PlayerState> pressPlay() const = 0;
    virtual std::string name() const = 0;
};

class PlayingState : public PlayerState
{
public:
    std::unique_ptr<PlayerState> pressPlay() const override;

    // trivial override
    std::string name() const override
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
    std::unique_ptr<PlayerState> pressPlay() const override
    {
        std::cout << "Resuming playback" << std::endl;
        return std::make_unique<PlayingState>();
    }

    // trivial override
    std::string name() const override
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
std::unique_ptr<PlayerState> PlayingState::pressPlay() const
{
    std::cout << "Pausing playback" << std::endl;
    return std::make_unique<PausedState>();
}

class MediaPlayer
{
public:
    // trivial constructor
    MediaPlayer() : state(std::make_unique<PausedState>()) {}

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
        std::cout << "Now in state: " << state->name() << std::endl;
    }

private:
    std::unique_ptr<PlayerState> state;
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
