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
#include "guise/font.hpp"
#include <thread>
#include <iostream>

using namespace Guise;

int main()
{
    Context::setDpiAware();

    auto context = Context::create();
    context->setMaxFrameTime(std::chrono::duration<double>(0.009f));
    auto appWindow1 = context->addAppWindow(L"Example 1", { 400, 400 });
    appWindow1->show();
    auto canvas1 = appWindow1->getCanvas();
    
  

    auto vertGrid1 = VerticalGrid::create();
    canvas1->add(vertGrid1);

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
    //button1_1->enableInput();

    auto button2 = Button::create();
    button2->setSize({ 300.0f, 80.0f });
    button2->onPress = []() { std::cout << "Pressed button 2!" << std::endl; };
    vertGrid1->add(button2);
    
    auto button3 = Button::create();
    button3->setSize({ 200.0f, 40.0f });
    //button3->getStyleHover().setSize({ 200.0f, 45.0f });
    button3->getStyleActive().setBorderWidth(5.0f);
    button3->onRelease = []() { std::cout << "Released button 3!" << std::endl; };
    vertGrid1->add(button3);

    auto label1 = Label::create("arial", L"Checkboxes!");
    label1->setFontSize(10);
    vertGrid1->add(label1);

    auto horiGrid1 = HorizontalGrid::create();
    vertGrid1->add(horiGrid1);
    //horiGrid1->setSize({ 200.0f, 40.0f });
    auto checkbox1 = Checkbox::create();
    checkbox1->setPosition({10.0, 0.0f});
    auto checkbox2 = Checkbox::create();
    checkbox2->getCheckedStyle().setSize({30.0f, 30.0f});
    horiGrid1->add(checkbox1);
    horiGrid1->add(checkbox2);
    horiGrid1->add(Checkbox::create());
    horiGrid1->add(Checkbox::create());
    horiGrid1->add(Checkbox::create());
    horiGrid1->add(Checkbox::create());
    horiGrid1->add(Checkbox::create());
    horiGrid1->add(Checkbox::create());
    horiGrid1->add(Checkbox::create());
    horiGrid1->add(Checkbox::create());

    auto button4 = Button::create();
    button4->setSize({ 200.0f, 40.0f });
    vertGrid1->add(button4);


    // Signal dependency tests.
    button1->onHover = { { button2 }, [&button2]()
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

    button1->onPress = { { button2 }, [&button2]()
    {
        button2->release();
        button2.reset();
    } };
    
    button1->onPress = { { button2 }, [&button2]()
    {
        std::cout << "test. " << std::endl;
    } };

    button2->onRelease = { { appWindow1 }, [&appWindow1]()
    {
        appWindow1->maximize();
    } };

    button3->onRelease = { { appWindow1 }, [&appWindow1]()
    {
        appWindow1->minimize();
    } };

    button4->onRelease = { { button3 }, [&button3]()
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
    
   
    





/*
    auto button1 = Button::create();
    button1->setSize({ 200.0f, 60.0f });

    auto button2 = Button::create();
    button2->setSize({ 300.0f, 80.0f });
    auto inButton = Button::create();
 
    button2->add(inButton);
    auto newPadding = button2->getPadding();
    newPadding.x = 100.0f;
    button2->setPadding(newPadding);
    //inButton->enableInput();
    inButton->add(Label::create("arial",   L"The quick brown fox jumps over the lazy dog."));
    auto button3 = Button::create();
    button3->setSize({ 200.0f, 40.0f });
    button3->setBorderWidth(1.0f);

    auto verticalGrid1 = VerticalGrid::create();
    verticalGrid1->getSlotStyle().setSize({ 0.0f, 0.0f });

    auto label1 = Label::create( "arial",  std::wstring(1, wchar_t(0x0ED4)) );
    //auto label2 = Label::create(canvas1, L"Hello world");

    //verticalGrid1->add(button1);
    verticalGrid1->add(button2);
    //verticalGrid1->add(button3);
    verticalGrid1->add(Label::create("arial", L"Jimmie o'boy"));
    button1->add(label1);
    //verticalGrid1->add(label2);

    canvas1->add(verticalGrid1);

    std::shared_ptr<const Control> a = inButton;
    auto childs = a->getChilds();

    button2->onPress = [](Vector2f val)
    {
        std::cout << "Button 2 is pressed! :" << val.x << " " << val.y << std::endl;
    };
    

    auto appWindow2 = context->addAppWindow(L"Example 2", { 400, 400 });
    auto canvas2 = appWindow2->getCanvas();
    canvas2->add(Button::create());
    */
    /*for (int i = 0; i < 100; i++)
    {
        std::this_thread::sleep_for(std::chrono::duration<double>(0.5f));

        if (i % 2)
        {
            label1->setFontSize(80);
            label1->setText(L"Hello");
        }
        else 
        {
            label1->setFontSize(40);
            label1->setText(L"World");
        }
    }*/
   

    // Wait for application window to close.
    Semaphore closeSemaphore;
    appWindow1->onClose = [&closeSemaphore]()
    {
        closeSemaphore.notifyOne();
    };
    closeSemaphore.wait();

    return 0;
}