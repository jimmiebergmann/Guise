#include "test.hpp"
#include "guise/lib_template.hpp"

TEST(Test, Template)
{

    LibTemplate libTemp;

    libTemp.setX(-1);
    libTemp.setY(5);

    EXPECT_EQ(libTemp.getSum(), 4);

}