#include <iostream>
#include <memory>

// Abstract Factory: families of related UI widgets (Light and Dark themes).
class Button
{
public:
    virtual ~Button() = default;
    virtual void paint() const = 0;
};

class Checkbox
{
public:
    virtual ~Checkbox() = default;
    virtual void paint() const = 0;
};

class LightButton : public Button
{
public:
    // trivial override
    void paint() const override
    {
        std::cout << "Rendering a light-themed button" << std::endl;
    }
};

class LightCheckbox : public Checkbox
{
public:
    // trivial override
    void paint() const override
    {
        std::cout << "Rendering a light-themed checkbox" << std::endl;
    }
};

class DarkButton : public Button
{
public:
    // trivial override
    void paint() const override
    {
        std::cout << "Rendering a dark-themed button" << std::endl;
    }
};

class DarkCheckbox : public Checkbox
{
public:
    // trivial override
    void paint() const override
    {
        std::cout << "Rendering a dark-themed checkbox" << std::endl;
    }
};

class WidgetFactory
{
public:
    virtual ~WidgetFactory() = default;
    virtual std::unique_ptr<Button> createButton() const = 0;
    virtual std::unique_ptr<Checkbox> createCheckbox() const = 0;
};

class LightFactory : public WidgetFactory
{
public:
    // trivial override
    std::unique_ptr<Button> createButton() const override
    {
        return std::make_unique<LightButton>();
    }

    // trivial override
    std::unique_ptr<Checkbox> createCheckbox() const override
    {
        return std::make_unique<LightCheckbox>();
    }
};

class DarkFactory : public WidgetFactory
{
public:
    // trivial override
    std::unique_ptr<Button> createButton() const override
    {
        return std::make_unique<DarkButton>();
    }

    // trivial override
    std::unique_ptr<Checkbox> createCheckbox() const override
    {
        return std::make_unique<DarkCheckbox>();
    }
};

/*****************************************************************************
 * Name: renderUi
 *
 * Description:
 *         Builds a button and a checkbox from the supplied factory and
 *         paints them, without knowing which theme is in use.
 *
 * Inputs:
 *         factory : the widget factory that determines the theme.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void renderUi(const WidgetFactory& factory)
{
    auto button = factory.createButton();
    auto checkbox = factory.createCheckbox();
    button->paint();
    checkbox->paint();
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Renders a UI using both the light and dark widget factories.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    LightFactory light;
    DarkFactory dark;
    renderUi(light);
    renderUi(dark);
    return 0;
}
