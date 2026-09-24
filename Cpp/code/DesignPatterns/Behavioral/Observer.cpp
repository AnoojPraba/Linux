#include <iostream>
#include <vector>

using namespace std;

// Observer: subscribers get notified when the weather station updates.
class Observer
{
public:
    virtual ~Observer() = default;
    virtual void update(double temperature) const = 0;
};

class PhoneDisplay : public Observer
{
public:
    /*****************************************************************************
     * Name: update
     *
     * Description:
     *         Reacts to a new temperature reading by printing it to the phone
     *         display.
     *
     * Inputs:
     *         temperature : the latest temperature reading.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void update(double temperature) const override
    {
        cout << "Phone display shows: " << temperature << " C" << endl;
    }
};

class WebDisplay : public Observer
{
public:
    // trivial override
    void update(double temperature) const override
    {
        cout << "Web display shows: " << temperature << " C" << endl;
    }
};

class WeatherStation
{
public:
    /*****************************************************************************
     * Name: attach
     *
     * Description:
     *         Registers an observer to receive future temperature updates.
     *
     * Inputs:
     *         observer : the observer to register.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void attach(Observer* observer)
    {
        observers.push_back(observer);
    }

    /*****************************************************************************
     * Name: setTemperature
     *
     * Description:
     *         Updates the station's temperature and notifies every registered
     *         observer of the new value.
     *
     * Inputs:
     *         temperature : the new temperature reading.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void setTemperature(double temperature)
    {
        for (const auto* observer : observers)
        {
            observer->update(temperature);
        }
    }

private:
    vector<Observer*> observers;
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Registers two displays with a weather station and pushes a
 *         temperature update to demonstrate both being notified.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    WeatherStation station;
    PhoneDisplay phone;
    WebDisplay web;
    station.attach(&phone);
    station.attach(&web);
    station.setTemperature(23.5);
    return 0;
}
