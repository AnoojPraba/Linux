#include <iostream>
#include <memory>

// Adapter: make an old media player interface work with a new player.
class ModernPlayer
{
public:
    virtual ~ModernPlayer() = default;
    virtual void playMp4(const std::string& file) const = 0;
};

// Legacy class with an incompatible interface we cannot change.
class LegacyAviPlayer
{
public:
    /*****************************************************************************
     * Name: playAvi
     *
     * Description:
     *         Plays a file using the legacy AVI-only API.
     *
     * Inputs:
     *         file : the name of the AVI file to play.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void playAvi(const std::string& file) const
    {
        std::cout << "Playing AVI file: " << file << std::endl;
    }
};

// Adapts LegacyAviPlayer to the ModernPlayer interface.
class AviToModernAdapter : public ModernPlayer
{
public:
    /*****************************************************************************
     * Name: playMp4
     *
     * Description:
     *         Adapts a modern playMp4 request into a legacy playAvi call so the
     *         legacy player can be used behind the modern interface.
     *
     * Inputs:
     *         file : the name of the file to play.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void playMp4(const std::string& file) const override
    {
        std::cout << "Adapter converting request for: " << file << std::endl;
        legacyPlayer.playAvi(file);
    }

private:
    LegacyAviPlayer legacyPlayer;
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Plays a file through the modern interface, backed by the legacy
 *         player via the adapter.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    std::unique_ptr<ModernPlayer> player = std::make_unique<AviToModernAdapter>();
    player->playMp4("vacation.mp4");
    return 0;
}
