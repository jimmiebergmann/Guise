#include "guise/build.hpp"
#include "guise/context.hpp"
#include "guise/appWindow.hpp"
#include "guise/control/button.hpp"
#include <thread>

using namespace Guise;

int main()
{
    auto context = Context<>::create();
    context->setMaxFrameTime(std::chrono::duration<double>(0.009f));
    auto appWindow1 = context->addAppWindow(L"Example 1", { 400, 400 });
    auto canvas1 = appWindow1->getCanvas();

    auto button1 = Button::create(*canvas1);
    button1->setPosition({ 100.0f, 100.0f });
    button1->setSize({ 600.0f, 600.0f });
    //button1->setSize({ 130.0f, 130.0f });

    auto current = button1;

    for (size_t i = 0; i < 10; i++)
    {
        auto newButton = Button::create(*canvas1);
        current->add(newButton);
        current = newButton;
    }

    

    

    /*button1->setStyleProperties(
        {   
            { Style::Property::Position,        { 100, 200 } },
            { Style::Property::Size,            { 300, 200 } },
            { Style::Property::Padding,         10.0f/*{ 2.0f, 5.0f, 10.0f, 15.0f }*//* } ,
            { Style::Property::BackgroundColor, { 0.0, 1.0f, 0.0, 1.0f } } ,
            { Style::Property::Border,          Style::BorderStyle::Solid } ,
            { Style::Property::BorderWidth,     10.0f } ,
            { Style::Property::BorderColor,     { 1.0f, 0.0, 0.0, 1.0f } }
        }
    );*/

    /*auto button2 = Button::create();  
    button2->setStyleProperties(
        { 
            { Style::Property::Size,            { 300, 200 } },
            { Style::Property::Padding,         50.0f },
            { Style::Property::BackgroundColor, { 0.22352f, 0.23529f, 0.25490f, 1.0f } } ,
            { Style::Property::Border,          Style::BorderStyle::None } ,
            { Style::Property::BorderWidth,     10.0f } ,
            { Style::Property::BorderColor,     { 0.0, 1.0f, 1.0, 1.0f } }
        }
    );



    button1->add(button2);*/

    canvas1->add(button1);

    /*std::this_thread::sleep_for(std::chrono::duration<double>(1.0f));
    auto appWindow2 = context->addAppWindow(L"Example 2", { 400, 200 });
    auto canvas2 = appWindow2->getCanvas();

    auto button3 = Button::create();
    button3->setStyleProperties(
        {
            { Style::Property::Size,            { 300, 100 } },
            { Style::Property::Padding,         50.0f },
            { Style::Property::BackgroundColor, { 0.22352f, 0.23529f, 0.25490f, 1.0f } } ,
            { Style::Property::Border,          Style::BorderStyle::None } ,
            { Style::Property::BorderWidth,     10.0f } ,
            { Style::Property::BorderColor,     { 0.0, 1.0f, 1.0, 1.0f } }
        }
    );

    canvas2->add(button3);*/
    
    std::this_thread::sleep_for(std::chrono::duration<double>(1000.0f));
    return 0;
}