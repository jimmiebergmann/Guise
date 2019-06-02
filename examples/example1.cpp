#include "guise/build.hpp"
#include "guise/context.hpp"
#include "guise/appWindow.hpp"
#include "guise/control/button.hpp"
#include "guise/control/movable.hpp"
#include "guise/control/verticalGrid.hpp"
#include "guise/control/label.hpp"
#include "guise/font.hpp"
#include <thread>
#include <iostream>

using namespace Guise;

int main()
{
    auto context = Context::create();
    context->setMaxFrameTime(std::chrono::duration<double>(0.009f));
    auto appWindow1 = context->addAppWindow(L"Example 1", { 400, 400 });
    auto canvas1 = appWindow1->getCanvas();
    
    auto button1 = Button::create(canvas1);
    button1->setSize({ 200.0f, 60.0f });

    auto button2 = Button::create(canvas1);
    button2->setSize({ 300.0f, 80.0f });
    auto inButton = Button::create(canvas1);

    button2->add(inButton);
    inButton->add(Label::create(canvas1, /*"Typeface.TTF"*/ "arial", /*L"ABCD"*/   L"The quick brown fox jumps over the lazy dog."));
    auto button3 = Button::create(canvas1);
    button3->setSize({ 200.0f, 40.0f });

    auto verticalGrid1 = VerticalGrid::create(canvas1);
    verticalGrid1->getSlotStyle().setSize({ 0.0f, 0.0f });

    auto label1 = Label::create(canvas1, /*"Typeface.TTF"*/ "arial", /*L"ABCD"*/   std::wstring(1, wchar_t(0x0ED4)) /*L"The quick brown fox jumps over the lazy dog."*/);
    //auto label2 = Label::create(canvas1, L"Hello world");

    verticalGrid1->add(button1);
    verticalGrid1->add(button2);
    verticalGrid1->add(button3);
    verticalGrid1->add(Label::create(canvas1, "arial", L"Jimmie o'boy"));
    button1->add(label1);
    //verticalGrid1->add(label2);

    canvas1->add(verticalGrid1);

    button2->onPressed = []()
    {
        std::cout << "Button 2 is pressed!" << std::endl;
    };


    for (int i = 0; i < 100; i++)
    {
        std::this_thread::sleep_for(std::chrono::duration<double>(0.5f));

        if (i % 2)
        {
            label1->setFontHeight(80);
            label1->setText(L"Hello");
        }
        else 
        {
            label1->setFontHeight(40);
            label1->setText(L"World");
        }
    }
    
    std::this_thread::sleep_for(std::chrono::duration<double>(1000.0f));
    return 0;
}