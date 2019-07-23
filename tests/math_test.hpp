#include "test.hpp"
#include "guise/math/vector.hpp"
#include "guise/math/matrix.hpp"
#include "guise/math/bounds.hpp"

using namespace Guise;

/*TEST(Vector, Vector2)
{

}


TEST(Matrix, Matrix44)
{

}*/


TEST(Bounds, Constructor)
{
    {
        {
            Bounds2f bounds1 = { { 1.0f, 2.0f }, { 3.0f, 4.0f } };
            EXPECT_EQ(bounds1.position.x, 1.0f);
            EXPECT_EQ(bounds1.position.y, 2.0f);
            EXPECT_EQ(bounds1.size.x, 3.0f);
            EXPECT_EQ(bounds1.size.y, 4.0f);

            Bounds2f bounds2 = { { 9.0f, 8.0f }, { 7.0f, 6.0f } };
            EXPECT_EQ(bounds2.position.x, 9.0f);
            EXPECT_EQ(bounds2.position.y, 8.0f);
            EXPECT_EQ(bounds2.size.x, 7.0f);
            EXPECT_EQ(bounds2.size.y, 6.0f);
        }
    }
    {
        {
            Bounds2f bounds1 = { 1.0f, 2.0f, 3.0f, 4.0f };
            EXPECT_EQ(bounds1.position.x, 1.0f);
            EXPECT_EQ(bounds1.position.y, 2.0f);
            EXPECT_EQ(bounds1.size.x, 3.0f);
            EXPECT_EQ(bounds1.size.y, 4.0f);

            Bounds2f bounds2 = { 9.0f, 8.0f, 7.0f, 6.0f };
            EXPECT_EQ(bounds2.position.x, 9.0f);
            EXPECT_EQ(bounds2.position.y, 8.0f);
            EXPECT_EQ(bounds2.size.x, 7.0f);
            EXPECT_EQ(bounds2.size.y, 6.0f);

            Bounds2f bounds3 = bounds1;
            EXPECT_EQ(bounds3.position.x, 1.0f);
            EXPECT_EQ(bounds3.position.y, 2.0f);
            EXPECT_EQ(bounds3.size.x, 3.0f);
            EXPECT_EQ(bounds3.size.y, 4.0f);

            bounds3 = bounds2;
            EXPECT_EQ(bounds3.position.x, 9.0f);
            EXPECT_EQ(bounds3.position.y, 8.0f);
            EXPECT_EQ(bounds3.size.x, 7.0f);
            EXPECT_EQ(bounds3.size.y, 6.0f);
        }
    }
    {
        {
            Bounds2i32 bounds1 = { { 1, 2 }, { 3, 4 } };
            EXPECT_EQ(bounds1.position.x, 1);
            EXPECT_EQ(bounds1.position.y, 2);
            EXPECT_EQ(bounds1.size.x, 3);
            EXPECT_EQ(bounds1.size.y, 4);

            Bounds2i32 bounds2 = { { 9, 8 }, { 7, 6 } };
            EXPECT_EQ(bounds2.position.x, 9);
            EXPECT_EQ(bounds2.position.y, 8);
            EXPECT_EQ(bounds2.size.x, 7);
            EXPECT_EQ(bounds2.size.y, 6);
        }
    }
    {
        {
            Bounds2i32 bounds1 = { 1, 2, 3, 4 };
            EXPECT_EQ(bounds1.position.x, 1);
            EXPECT_EQ(bounds1.position.y, 2);
            EXPECT_EQ(bounds1.size.x, 3);
            EXPECT_EQ(bounds1.size.y, 4);

            Bounds2i32 bounds2 = { 9, 8, 7, 6 };
            EXPECT_EQ(bounds2.position.x, 9);
            EXPECT_EQ(bounds2.position.y, 8);
            EXPECT_EQ(bounds2.size.x, 7);
            EXPECT_EQ(bounds2.size.y, 6);

            Bounds2f bounds3 = bounds1;
            EXPECT_EQ(bounds3.position.x, 1);
            EXPECT_EQ(bounds3.position.y, 2);
            EXPECT_EQ(bounds3.size.x, 3);
            EXPECT_EQ(bounds3.size.y, 4);

            bounds3 = bounds2;
            EXPECT_EQ(bounds3.position.x, 9);
            EXPECT_EQ(bounds3.position.y, 8);
            EXPECT_EQ(bounds3.size.x, 7);
            EXPECT_EQ(bounds3.size.y, 6);
        }
    }
}

TEST(Bounds, Compare)
{
    {
        Bounds2i32 bounds1 = { 5, 6, 7, 8 };
        Bounds2i32 bounds2 = { 5, 6, 7, 8 };

        Bounds2i32 bounds3 = { 5, 6, 7, 0 };
        Bounds2i32 bounds4 = { 5, 6, 1, 8 };
        Bounds2i32 bounds5 = { 5, 20, 7, 8 };
        Bounds2i32 bounds6 = { 3, 6, 7, 8 };

        EXPECT_TRUE(bounds1 == bounds2);
        EXPECT_TRUE(bounds2 == bounds1);
        EXPECT_FALSE(bounds1 != bounds2);
        EXPECT_FALSE(bounds2 != bounds1);

        EXPECT_TRUE(bounds1 != bounds3);
        EXPECT_TRUE(bounds1 != bounds4);
        EXPECT_TRUE(bounds1 != bounds5);
        EXPECT_TRUE(bounds1 != bounds6);
        EXPECT_FALSE(bounds1 == bounds3);
        EXPECT_FALSE(bounds1 == bounds4);
        EXPECT_FALSE(bounds1 == bounds5);
        EXPECT_FALSE(bounds1 == bounds6);

        EXPECT_TRUE(bounds3 != bounds1);
        EXPECT_TRUE(bounds4 != bounds1);
        EXPECT_TRUE(bounds5 != bounds1);
        EXPECT_TRUE(bounds6 != bounds1);
        EXPECT_FALSE(bounds3 == bounds1);
        EXPECT_FALSE(bounds4 == bounds1);
        EXPECT_FALSE(bounds5 == bounds1);
        EXPECT_FALSE(bounds6 == bounds1);
    }
}

TEST(Bounds, Transform)
{
    {
        {
            Bounds2i32 bounds = { {0, 0}, {100, 200} };
            EXPECT_EQ(bounds, Bounds2i32({ 0, 0 }, { 100, 200 }));
            EXPECT_EQ(bounds.cutTop(40), Bounds2i32({ 0, 40 }, { 100, 160 }));
        }
        {
            Bounds2i32 bounds = { { 50, 100 }, { 200, 300 } };
            EXPECT_EQ(bounds, Bounds2i32({ 50, 100 }, { 200, 300 }));
            EXPECT_EQ(bounds.cutTop(50), Bounds2i32({ 50, 150 }, { 200, 250 }));
        }
        {
            Bounds2i32 bounds = { { 50, 100 },{ 200, 300 } };
            EXPECT_EQ(bounds, Bounds2i32({ 50, 100 }, { 200, 300 }));
            EXPECT_EQ(bounds.cutTop(400), Bounds2i32({ 50, 400 }, { 200, 0 }));
        }
    }
    {
        {
            Bounds2i32 bounds = { { 0, 0 },{ 100, 200 } };
            EXPECT_EQ(bounds.cutLeft(40), Bounds2i32({ 40, 0 }, { 60, 200 }));
        }
        {
            Bounds2i32 bounds = { { 50, 100 },{ 200, 300 } };
            EXPECT_EQ(bounds.cutLeft(50), Bounds2i32({ 100, 100 }, { 150, 300 }));
        }
        {
            Bounds2i32 bounds = { { 50, 100 },{ 200, 300 } };
            EXPECT_EQ(bounds.cutLeft(400), Bounds2i32({ 250, 100 }, { 0, 300 }));
        }
    }
    {
        {
            Bounds2i32 bounds = { { 0, 0 },{ 100, 200 } };
            EXPECT_EQ(bounds.cutEdges(10), Bounds2i32({ 10, 10 }, { 80, 180 }));
        }
        {
            Bounds2i32 bounds = { { 50, 100 },{ 200, 300 } };
            EXPECT_EQ(bounds.cutEdges(20), Bounds2i32({ 70, 120 }, { 160, 260 }));
        }
        /*{
            Bounds2i32 bounds = { { 50, 100 },{ 200, 300 } };
            EXPECT_EQ(bounds.cutEdges(200), Bounds2i32({ 150, 200 }, { 0, 100 }));
        }
        {
            Bounds2i32 bounds = { { 50, 100 },{ 400, 400 } };
            EXPECT_EQ(bounds.cutEdges(1000), Bounds2i32({ 250, 300 }, { 0, 0 }));
        }
        {
            Bounds2i32 bounds = { { 50, 100 },{ 400, 200 } };
            EXPECT_EQ(bounds.cutEdges(1000), Bounds2i32({ 150, 200 }, { 200, 0 }));
        }
        {
            Bounds2i32 bounds = { { 50, 100 },{ 200, 400 } };
            EXPECT_EQ(bounds.cutEdges(1000), Bounds2i32({ 150, 200 }, { 0, 200 }));
        }*/
    }
    {
        {
            Bounds2i32 bounds = { { 0, 0 },{ 100, 200 } };
            EXPECT_EQ(bounds.cutEdges({10, 20}), Bounds2i32({ 10, 20 }, { 80, 160 }));
        }
        {
            Bounds2i32 bounds = { { 50, 100 },{ 300, 300 } };
            EXPECT_EQ(bounds.cutEdges({ 30, 40 }), Bounds2i32({ 80, 140 }, { 240, 220 }));
        }
        /*{
            Bounds2i32 bounds = { { 50, 100 },{ 400, 200 } };
            EXPECT_EQ(bounds.cutEdges({ 1000, 50 }), Bounds2i32({ 250, 150 }, { 0, 100 }));
            EXPECT_EQ(1, 1);
        }
        {
            Bounds2i32 bounds = { { 50, 100 },{ 400, 200 } };
            EXPECT_EQ(bounds.cutEdges({ 1000, 500 }), Bounds2i32({ 250, 150 }, { 200, 0 }));
            EXPECT_EQ(1, 1);
        }*/

    }
}