#include <gtest/gtest.h>
#include "polynom.h"

TEST(Polynomial_x_test, test_polynom_1)
{
    constexpr Polynomial<5> pol(std::array<int, 5> { 1, 2, 3, 4, 5 });
    constexpr int calculated_num = pol.calculate_x(1);
    ASSERT_EQ(calculated_num, 15) << "Wrong answer with x=1! Coefficients: { 1, 2, 3, 4, 5 }";
}

TEST(Polynomial_x_test, test_polynom_2)
{
    constexpr Polynomial<4> pol2(std::array<int, 4> { 0, 3, 5, 7 });
    constexpr int calculated_num = pol2.calculate_x(2);
    ASSERT_EQ(calculated_num, 82) << "Wrong answer with x=2! Coefficients: { 0, 3, 5, 7 }";
}

TEST(Polynomial_x_test, test_zero_polynom_3)
{
    constexpr Polynomial<4> pol(std::array<int, 4> { 0, 0, 0, 0});
    constexpr int calculated_num = pol.calculate_x(100);
    ASSERT_EQ(calculated_num, 0) << "Wrong answer with x=100! Coefficients: { 0, 0, 0, 0 }";
}

int main(int argc, char **argv) {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}