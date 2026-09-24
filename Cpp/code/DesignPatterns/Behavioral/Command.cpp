#include <iostream>

using namespace std;

// Command: encapsulate light on/off requests as objects for a remote control.
class Light
{
public:
    // trivial
    void on()
    {
        cout << "Light is ON" << endl;
    }

    // trivial
    void off()
    {
        cout << "Light is OFF" << endl;
    }
};

class Command
{
public:
    virtual ~Command() = default;
    virtual void execute() const = 0;
};

class LightOnCommand : public Command
{
public:
    // trivial constructor
    explicit LightOnCommand(Light& light) : light(light) {}

    // trivial override
    void execute() const override
    {
        light.on();
    }

private:
    Light& light;
};

class LightOffCommand : public Command
{
public:
    // trivial constructor
    explicit LightOffCommand(Light& light) : light(light) {}

    // trivial override
    void execute() const override
    {
        light.off();
    }

private:
    Light& light;
};

class RemoteControl
{
public:
    /*****************************************************************************
     * Name: pressButton
     *
     * Description:
     *         Executes the given command, invoking whatever action it
     *         encapsulates.
     *
     * Inputs:
     *         command : the command to execute.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void pressButton(const Command& command) const
    {
        command.execute();
    }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Uses a remote control to execute on/off commands against a light,
 *         showing the invoker is decoupled from the receiver.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    Light livingRoomLight;
    LightOnCommand turnOn(livingRoomLight);
    LightOffCommand turnOff(livingRoomLight);
    RemoteControl remote;

    remote.pressButton(turnOn);
    remote.pressButton(turnOff);
    return 0;
}
