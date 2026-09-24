#include <iostream>
#include <string>
#include <vector>

using namespace std;

class ChatUser;

// Mediator: routes messages between colleagues so they never reference
// each other directly.
class ChatRoomMediator
{
public:
    virtual ~ChatRoomMediator() = default;
    virtual void addUser(ChatUser* user) = 0;
    virtual void broadcast(const ChatUser& sender, const string& message) const = 0;
};

// Colleague: only knows about the mediator, not other colleagues.
class ChatUser
{
public:
    ChatUser(string nameIn, ChatRoomMediator& mediatorIn)
        : userName(std::move(nameIn)), mediator(mediatorIn) {}

    // trivial getter
    const string& getName() const
    {
        return userName;
    }

    /*****************************************************************************
     * Name: send
     *
     * Description:
     *         Sends a message by handing it to the mediator, which forwards
     *         it to every other registered user.
     *
     * Inputs:
     *         message : the text to send.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void send(const string& message) const
    {
        mediator.broadcast(*this, message);
    }

    /*****************************************************************************
     * Name: receive
     *
     * Description:
     *         Prints a message that was delivered by the mediator.
     *
     * Inputs:
     *         from    : the sender's name.
     *         message : the delivered text.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void receive(const string& from, const string& message) const
    {
        cout << userName << " received from " << from << ": " << message << endl;
    }

private:
    string userName;
    ChatRoomMediator& mediator;
};

class ChatRoom : public ChatRoomMediator
{
public:
    // trivial registration
    void addUser(ChatUser* user) override
    {
        users.push_back(user);
    }

    /*****************************************************************************
     * Name: broadcast
     *
     * Description:
     *         Delivers a message from one user to every other user
     *         currently registered in the chat room.
     *
     * Inputs:
     *         sender  : the user sending the message.
     *         message : the text being sent.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void broadcast(const ChatUser& sender, const string& message) const override
    {
        for (const auto* user : users)
        {
            if (user->getName() != sender.getName())
            {
                user->receive(sender.getName(), message);
            }
        }
    }

private:
    vector<ChatUser*> users;
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Registers three users with a chat room mediator and sends a
 *         message to show it fanned out without users referencing
 *         each other directly.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    ChatRoom room;
    ChatUser alice("Alice", room);
    ChatUser bob("Bob", room);
    ChatUser carol("Carol", room);

    room.addUser(&alice);
    room.addUser(&bob);
    room.addUser(&carol);

    alice.send("Hello everyone!");
    return 0;
}
