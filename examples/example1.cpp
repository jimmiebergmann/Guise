#include "guise/build.hpp"
#include "guise/context.hpp"
#include "guise/appWindow.hpp"
#include "guise/control/button.hpp"
#include "guise/control/movable.hpp"
#include "guise/control/verticalGrid.hpp"
#include "guise/control/horizontalGrid.hpp"
#include "guise/control/label.hpp"
#include "guise/control/textBox.hpp"
#include "guise/control/checkbox.hpp"
#include "guise/control/tabWindow.hpp"
#include "guise/font.hpp"
#include <thread>
#include <iostream>

using namespace Guise;

static void initExampleWindow(std::shared_ptr<AppWindow> & appWindow);

int main()
{
    Context::setDpiAware();

    auto context = Context::create();
    context->setMaxFrameTime(std::chrono::duration<double>(0.009f));
    
    // Create example 1 window.
    auto appWindow = context->addAppWindow(L"Example 1", { 500, 500 });
    initExampleWindow(appWindow);
    appWindow->show();

   
    // Wait for application window to close.
    Semaphore closeSemaphore;
    appWindow->onClose = [&closeSemaphore]()
    {
        closeSemaphore.notifyOne();
    };
    closeSemaphore.wait();

    return 0;
}

void initExampleWindow(std::shared_ptr<AppWindow> & appWindow)
{
    auto canvas = appWindow->getCanvas();

    auto plane1 = Plane::create();
    canvas->add(plane1);

    auto vertGrid1 = VerticalGrid::create();
    plane1->add(vertGrid1);

    auto button1 = Button::create();
    button1->setSize({ 200.0f, 60.0f });
    button1->getStyleHover().setPadding(5.0f);
    vertGrid1->add(button1);

    auto button1_1 = Button::create();
    button1_1->setSize({ 30.0f, 0.0f });

    //button1_1->add(Label::create(canvas1, 
    //"/usr/share/fonts/truetype/freefont/freesans.ttf"¨
    //"arial", L"The quick brown fox jumps over the lazy dog."));
    button1->add(button1_1);
    button1->add(button1_1);
    //button1_1->enableInput();

    auto button2 = Button::create();
    button2->setSize({ 300.0f, 80.0f });
    button2->onPress = []() { std::cout << "Pressed button 2!" << std::endl; };
    vertGrid1->add(button2);

    auto button3 = Button::create();
    button3->setSize({ 200.0f, 40.0f });
    button3->getStyleHover().setSize({ 200.0f, 45.0f });
    button3->getStyleActive().setBorderWidth(5.0f);
    button3->onRelease = []() { std::cout << "Released button 3!" << std::endl; };
    vertGrid1->add(button3);

    auto label1 = Label::create("arial", L"Checkboxes!");
    label1->setFontSize(10);
    vertGrid1->add(label1);

    vertGrid1->add(TextBox::create());

    auto horiGrid1 = HorizontalGrid::create();
    vertGrid1->add(horiGrid1);
    auto checkbox1 = Checkbox::create();
    checkbox1->setPosition({ 10.0, 0.0f });
    auto checkbox2 = Checkbox::create();
    checkbox2->getCheckedStyle().setSize({ 30.0f, 30.0f });
    horiGrid1->add(checkbox1);
    horiGrid1->add(checkbox1);
    horiGrid1->add(checkbox2);
    for (int i = 0; i < 8; i++)
    {
        horiGrid1->add(Checkbox::create());
    }

    auto button4 = Button::create();
    button4->setSize({ 200.0f, 40.0f });
    vertGrid1->add(button4);

    auto tabWindow = TabWindow::create();
    canvas->add(tabWindow);
    tabWindow->setPosition({ 200.0f, 100.0f });
    //tabWindow->setLevel(100);


    // Signal dependency tests.
    button1->onHover = { { button2 }, [button2]()
    {
        if (button2->getBackgroundColor().x == 1.0f)
        {
            button2->setBackgroundColor({ 0.0f, 1.0f, 0.0f, 1.0f });
        }
        else
        {
            button2->setBackgroundColor({ 1.0f, 0.0f, 0.0f, 1.0f });
        }
    } };

    button1->onPress = { { button2 }, [button2]()
    {
        std::cout << "test 1" << std::endl;
        button2->release();
    } };

    button1->onPress = { { button2 }, [button2]()
    {
        std::cout << "test 2" << std::endl;
    } };

    button2->onRelease = { { appWindow }, [appWindow]()
    {
        appWindow->maximize();
    } };

    button3->onRelease = { { appWindow }, [appWindow]()
    {
        appWindow->minimize();
    } };

    button4->onRelease = { { button3 }, [button3]()
    {
        if (button3->getSize().y == 40.0f)
        {
            button3->setSize({ button3->getSize().x, 50.0f, });
        }
        else
        {
            button3->setSize({ button3->getSize().x, 40.0f, });
        }

    } };

}