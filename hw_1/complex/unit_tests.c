#include "complex.h"
#include <math.h>
#include "gtest/gtest.h"

namespace {

    TEST(Complex, Add) {
    	Complex a = {10, 11};
    	Complex b = {12, 13};
	EXPECT_EQ(add(a,b).real,22);
      	EXPECT_EQ(add(a,b).im,24);
    }

    TEST(Complex, Negate) {
    	Complex a = {10, 11};
    	EXPECT_EQ(negate(a).real,-10);
      	EXPECT_EQ(negate(a).im,-11);
    }

    TEST(Complex, Multiply) {
    	Complex a = {10, 11};
    	Complex b = {11, 12};
	EXPECT_EQ(multiply(a,b).real,-22);
      	EXPECT_EQ(multiply(a,b).im,241);

    }

    TEST(Complex, Magnitude) {
    	Complex a = {5, 12};
	EXPECT_EQ(magnitude(a),13);
    }
}
