#include "test.hpp"
#include "window_test.hpp"
#include "button_test.hpp"
#include "docking_test.hpp"

int main(int argc, char ** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
