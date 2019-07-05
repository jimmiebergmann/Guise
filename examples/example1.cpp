#include "guise/build.hpp"
#include "guise/context.hpp"
#include "guise/appWindow.hpp"
#include "guise/control/button.hpp"
#include "guise/control/movable.hpp"
#include "guise/control/verticalGrid.hpp"
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
    auto canvas1 = appWindow1->getCanvas();
    
    auto vertGrid1 = VerticalGrid::create(canvas1);
    canvas1->add(vertGrid1);

    auto button1 = Button::create(canvas1);
    button1->setSize({ 200.0f, 60.0f });
    button1->getHoverStyle().setPadding(5.0f);
    vertGrid1->add(button1);
    
    auto button1_1 = Button::create(canvas1);
    //button1_1->add(Label::create(canvas1, /*"/usr/share/fonts/truetype/freefont/freesans.ttf"¨*/"arial", L"The quick brown fox jumps over the lazy dog."));
    button1->add(button1_1);

    auto button2 = Button::create(canvas1);
    button2->setSize({ 300.0f, 80.0f });
    button2->onPressed = []() { std::cout << "Pressed button 2!" << std::endl; };
    vertGrid1->add(button2);   

    auto button3 = Button::create(canvas1);
    button3->setSize({ 200.0f, 40.0f });
    button3->getActiveStyle().setBorderWidth(5.0f);
    button3->onReleased = []() { std::cout << "Released button 3!" << std::endl; };
    vertGrid1->add(button3);

    auto label1 = Label::create(canvas1, "arial", L"Name");
    label1->setFontSize(10);
    vertGrid1->add(label1);

    vertGrid1->add(Checkbox::create(canvas1));

    auto textbox1 = TextBox::create(canvas1);
    vertGrid1->add(textbox1);
    //auto textbox2 = TextBox::create(canvas1);
    //vertGrid1->add(textbox2);

    /*button1->onPressed = []()
    {
        std::cout << "Lamda: Pressed button." << std::endl;
    };

    button1->onReleased = []()
    {
        std::cout << "Lamda: Release button." << std::endl;
    };

    button1->onHover = []()
    {
        std::cout << "Lamda: hover button." << std::endl;
    };*/




/*
    auto button1 = Button::create(canvas1);
    button1->setSize({ 200.0f, 60.0f });

    auto button2 = Button::create(canvas1);
    button2->setSize({ 300.0f, 80.0f });
    auto inButton = Button::create(canvas1);
 
    button2->add(inButton);
    auto newPadding = button2->getPadding();
    newPadding.x = 100.0f;
    button2->setPadding(newPadding);
    //inButton->enableInput();
    inButton->add(Label::create(canvas1, "arial",   L"The quick brown fox jumps over the lazy dog."));
    auto button3 = Button::create(canvas1);
    button3->setSize({ 200.0f, 40.0f });
    button3->setBorderWidth(1.0f);

    auto verticalGrid1 = VerticalGrid::create(canvas1);
    verticalGrid1->getSlotStyle().setSize({ 0.0f, 0.0f });

    auto label1 = Label::create(canvas1,  "arial",  std::wstring(1, wchar_t(0x0ED4)) );
    //auto label2 = Label::create(canvas1, L"Hello world");

    //verticalGrid1->add(button1);
    verticalGrid1->add(button2);
    //verticalGrid1->add(button3);
    verticalGrid1->add(Label::create(canvas1, "arial", L"Jimmie o'boy"));
    button1->add(label1);
    //verticalGrid1->add(label2);

    canvas1->add(verticalGrid1);

    std::shared_ptr<const Control> a = inButton;
    auto childs = a->getChilds();

    button2->onPressed = [](Vector2f val)
    {
        std::cout << "Button 2 is pressed! :" << val.x << " " << val.y << std::endl;
    };*/
    

   /* auto appWindow2 = context->addAppWindow(false, L"Example 2", { 400, 400 });
    auto canvas2 = appWindow2->getCanvas();
    canvas2->add(Button::create(canvas2));
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
    
    std::this_thread::sleep_for(std::chrono::duration<double>(1000.0f));
    return 0;
}