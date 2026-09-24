#include <iostream>
#include <memory>

using namespace std;

// Factory Method: create notifications without exposing the concrete class.
class Notification
{
public:
    virtual ~Notification() = default;
    virtual void send(const string& message) const = 0;
};

class EmailNotification : public Notification
{
public:
    // trivial override
    void send(const string& message) const override
    {
        cout << "Email: " << message << endl;
    }
};

class SmsNotification : public Notification
{
public:
    // trivial override
    void send(const string& message) const override
    {
        cout << "SMS: " << message << endl;
    }
};

enum class NotificationType
{
    EMAIL,
    SMS
};

/*****************************************************************************
 * Name: createNotification
 *
 * Description:
 *         Factory method that instantiates the concrete notification matching
 *         the requested type.
 *
 * Inputs:
 *         type : the kind of notification to create.
 *
 * Returns:
 *         A unique_ptr owning the newly created Notification.
 *****************************************************************************/
unique_ptr<Notification> createNotification(NotificationType type)
{
    if (type == NotificationType::EMAIL)
    {
        return make_unique<EmailNotification>();
    }
    return make_unique<SmsNotification>();
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Requests two different notification types from the factory method
 *         and sends a message through each.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    auto email = createNotification(NotificationType::EMAIL);
    auto sms = createNotification(NotificationType::SMS);
    email->send("Your order has shipped");
    sms->send("Your order has shipped");
    return 0;
}
