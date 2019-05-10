#include "guise/build.hpp"
#include "guise/context.hpp"
#include "guise/appWindow.hpp"
#include "guise/control/button.hpp"
#include <thread>

using namespace Guise;

int main()
{
    auto context = Context<>::create();
    context->setMaxFrameTime(std::chrono::duration<double>(0.01f));
    auto appWindow1 = context->addAppWindow(L"Example 1", { 800, 600 });
    auto canvas1 = appWindow1->getCanvas();

    auto button1 = Button::create();
    button1->setPosition({ 100, 200 });
    button1->setSize({ 300, 200 });
    button1->setPadding(50.0f);
    auto button2 = Button::create();
    //button2->setPosition({ 30, -60 });
    //button2->setSize({ 300, 100 });

    button1->add(button2);


    canvas1->add(button1);

    /*std::this_thread::sleep_for(std::chrono::duration<double>(1.0f));
    auto appWindow2 = context->addAppWindow(L"Example 2", { 400, 200 });
    */
    std::this_thread::sleep_for(std::chrono::duration<double>(1000.0f));
    return 0;
}