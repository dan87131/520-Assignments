#include <math.h>
#include <float.h> /* defines DBL_EPSILON */
#include <assert.h>
#include "typed_array.h"
#include "point.h"
#include "gtest/gtest.h"

namespace {

    TEST(TypedArray, Construction) {
        TypedArray<Point> b;
        b.set(0, Point(1, 2, 3));
        b.set(1, Point(4, 5, 6));
        b.set(2, Point(7, 8, 9));
        EXPECT_EQ(b.get(0).x, 1);
        EXPECT_EQ(b.get(1).y, 5);
        EXPECT_EQ(b.get(2).z, 9);
    }

    TEST(TypedArray, Defaults) {
        TypedArray<Point> x;
        Point& y = x.get(3);
        std::cout << x << "\n";
        EXPECT_DOUBLE_EQ(y.magnitude(), 0.0);
    }

    TEST(TypedArray, Matrix) {

        TypedArray<TypedArray<double>> m;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                m.get(i).set(j, 3 * i + j);
            }
        }

        std::cout << m << "\n";

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                EXPECT_DOUBLE_EQ(m.get(i).get(j), 3 * i + j);
            }
        }

    }

    TEST(TypedArray, CopyElementsInSet1) {
        TypedArray<Point> b;
        Point p(1, 2, 3);
        b.set(0, p);
        p.x = 4;
        EXPECT_DOUBLE_EQ(b.get(0).x, 1);
    }

    TEST(TypedArray, CopyElementsInSet2) {
        TypedArray<TypedArray<double>> m;
        TypedArray<double> x;
        x.set(0, 0);
        m.set(0, x);
        x.set(0, -1);
        EXPECT_DOUBLE_EQ(m.get(0).get(0), 0.0); //if set didn't make a copy, then would expect m[0][0]
    }


}