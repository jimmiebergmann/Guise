#include "test.hpp"
#include "guise/control/button.hpp"

using namespace Guise;

TEST(Control, Button)
{
   {
        auto button1 = Button::create();
        EXPECT_TRUE(button1->getParent().expired());

        auto button2 = Button::create();
        EXPECT_TRUE(button2->getParent().expired());

        {
            button1->add(button2);
            EXPECT_FALSE(button2->getParent().expired());
            EXPECT_EQ(button1->getChilds().size(), size_t(1));

            button1->remove(0);
            EXPECT_TRUE(button2->getParent().expired());
            EXPECT_EQ(button1->getChilds().size(), size_t(0));
        }
        {
            button1->add(button2);
            EXPECT_FALSE(button2->getParent().expired());
            EXPECT_EQ(button1->getChilds().size(), size_t(1));

            button1->remove(button2);
            EXPECT_TRUE(button2->getParent().expired());
            EXPECT_EQ(button1->getChilds().size(), size_t(0));
        }
        {
            button1->add(button2);
            EXPECT_FALSE(button2->getParent().expired());
            EXPECT_EQ(button1->getChilds().size(), size_t(1));

            EXPECT_EQ(button1->removeAll(), size_t(1));
            EXPECT_TRUE(button2->getParent().expired());
            EXPECT_EQ(button1->getChilds().size(), size_t(0));
        }
    }
    {
        auto button1 = Button::create();
        auto button2 = Button::create();
        auto button3 = Button::create();

        {
            EXPECT_EQ(button1->getChilds().size(), size_t(0));
            EXPECT_TRUE(button1->getParent().expired());
            EXPECT_TRUE(button2->getParent().expired());
            EXPECT_TRUE(button3->getParent().expired());
            
            button1->add(button2);
            EXPECT_EQ(button1->getChilds().size(), size_t(1));
            EXPECT_TRUE(button1->getParent().expired());
            EXPECT_FALSE(button2->getParent().expired());
            EXPECT_TRUE(button3->getParent().expired());
            
            button1->add(button3);
            EXPECT_EQ(button1->getChilds().size(), size_t(2));
            EXPECT_TRUE(button1->getParent().expired());
            EXPECT_FALSE(button2->getParent().expired());
            EXPECT_FALSE(button3->getParent().expired());

            EXPECT_EQ(button1->removeAll(), size_t(2));
            EXPECT_EQ(button1->getChilds().size(), size_t(0));
            EXPECT_TRUE(button1->getParent().expired());
            EXPECT_TRUE(button2->getParent().expired());
            EXPECT_TRUE(button3->getParent().expired());
        }
    }
    {
        auto button1 = Button::create();
        
        {
            auto button2 = Button::create();
            button1->add(button2);
            EXPECT_EQ(button1->getChilds().size(), size_t(1));

            {
                auto button3 = Button::create();
                button1->add(button3);
                EXPECT_EQ(button1->getChilds().size(), size_t(2));
            }

            EXPECT_EQ(button1->getChilds().size(), size_t(2));
            
        }

        EXPECT_EQ(button1->getChilds().size(), size_t(2));
        EXPECT_TRUE(button1->remove(1));
        EXPECT_EQ(button1->getChilds().size(), size_t(1));
        EXPECT_TRUE(button1->remove(0));
        EXPECT_EQ(button1->getChilds().size(), size_t(0));
        EXPECT_FALSE(button1->remove(0));
    }
}