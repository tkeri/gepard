/* Copyright (C) 2016, Gepard Graphics
 * Copyright (C) 2016, Szilard Ledan <szledan@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the FreeBSD Project.
 */

#include "gepard-types.h"
#include "gtest/gtest.h"

namespace {

/* Float tests */

TEST(FloatTest, ConstValuePrecisionOfFloat)
{
    ASSERT_NE(0.0, gepard::precisionOfFloat) << "The precisionOfFloat value is 0.";
    EXPECT_LT(0.0, gepard::precisionOfFloat) << "The precisionOfFloat is non-pozitive.";
    ASSERT_NE(INFINITY, gepard::precisionOfFloat) << "The precisionOfFloat is INFINITY.";
    ASSERT_NE(NAN, gepard::precisionOfFloat) << "The precisionOfFloat is NAN.";
}

TEST(FloatTest, FixPrecision)
{
    gepard::Float g = 1.0 / gepard::precisionOfFloat;
    gepard::Float h = 1.99 / gepard::precisionOfFloat;
    EXPECT_EQ(gepard::fixPrecision(g), gepard::fixPrecision(h)) << "The fixPrecision() rounds wrong.";

    g = 0.0 / gepard::precisionOfFloat;
    h = 0.99 / gepard::precisionOfFloat;
    EXPECT_EQ(gepard::fixPrecision(g), gepard::fixPrecision(h)) << "The fixPrecision() rounds wrong on positive numbers.";

    g = -1.0 / gepard::precisionOfFloat;
    h = -0.99 / gepard::precisionOfFloat;
    EXPECT_EQ(gepard::fixPrecision(g), gepard::fixPrecision(h)) << "The fixPrecision() rounds wrong on negative numbers.";
}

TEST(FloatTest, PiFloat)
{
    gepard::Float pi100 = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679;
    gepard::Float pi = gepard::fixPrecision(pi100);
    gepard::Float gepardPi = gepard::fixPrecision(gepard::piFloat);
    EXPECT_EQ(pi, gepardPi) << "Architecture used 'pi' is different than expected one.";
}

/* Basic math functions tests */

TEST(BasicMathFuncTest, Min)
{
    EXPECT_EQ(int(0), gepard::min(int(0), int(1))) << "The min<int> works wrong.";
    EXPECT_EQ(int(-1), gepard::min(int(-1), int(0))) << "The min<int> works wrong.";
    EXPECT_EQ(int(0), gepard::min(int(0), int(-0))) << "The min<int> works wrong.";
    EXPECT_EQ(gepard::Float(0.0), gepard::min(gepard::Float(0.0), gepard::Float(1.0))) << "The min<Float> works wrong.";
    EXPECT_EQ(gepard::Float(-1.0), gepard::min(gepard::Float(-1.0), gepard::Float(0.0))) << "The min<Float> works wrong.";
    EXPECT_EQ(gepard::Float(0.0), gepard::min(gepard::Float(0.0), gepard::Float(-0.0))) << "The min<Float> works wrong.";
}

TEST(BasicMathFuncTest, Max)
{
    EXPECT_EQ(int(1), gepard::max(int(0), int(1))) << "The max<int> works wrong.";
    EXPECT_EQ(int(0), gepard::max(int(-1), int(0))) << "The max<int> works wrong.";
    EXPECT_EQ(int(0), gepard::max(int(0), int(-0))) << "The max<int> works wrong.";
    EXPECT_EQ(gepard::Float(1.0), gepard::max(gepard::Float(0.0), gepard::Float(1.0))) << "The max<Float> works wrong.";
    EXPECT_EQ(gepard::Float(0.0), gepard::max(gepard::Float(-1.0), gepard::Float(0.0))) << "The max<Float> works wrong.";
    EXPECT_EQ(gepard::Float(0.0), gepard::max(gepard::Float(0.0), gepard::Float(-0.0))) << "The max<Float> works wrong.";
}

TEST(BasicMathFuncTest, Clamp)
{
    EXPECT_EQ(int(-1), gepard::clamp(int(-2), int(-1), int(1))) << "The clamp<int> works wrong.";
    EXPECT_EQ(int(0), gepard::clamp(int(-0), int(-1), int(1))) << "The clamp<int> works wrong.";
    EXPECT_EQ(int(1), gepard::clamp(int(2), int(-1), int(1))) << "The clamp<int> works wrong.";
    EXPECT_EQ(gepard::Float(-1.0), gepard::clamp(gepard::Float(-2.0), gepard::Float(-1.0), gepard::Float(1.0))) << "The clamp<Float> works wrong.";
    EXPECT_EQ(gepard::Float(0.0), gepard::clamp(gepard::Float(-0.0), gepard::Float(-1.0), gepard::Float(1.0))) << "The clamp<Float> works wrong.";
    EXPECT_EQ(gepard::Float(1.0), gepard::clamp(gepard::Float(2.0), gepard::Float(-1.0), gepard::Float(1.0))) << "The clamp<Float> works wrong.";
}

/* Region tests */

TEST(RegionTest, SizeTooBig)
{
    gepard::Region region;
    void* ptr = region.alloc(REGION_BLOCK_SIZE + 1);

    EXPECT_EQ(nullptr, ptr) << "The alloc() doesn't return 'nullptr' when 'size' is bigger than REGION_BLOCK_SIZE.";
}

TEST(RegionTest, SizeIsZero)
{
    gepard::Region region;
    void* ptr1 = region.alloc(0);
    void* ptr2 = region.alloc(0);

    EXPECT_EQ(ptr1, ptr2) << "The returned pointers are different despite 'size' was 0 in both allocations.";
}

} // anonymous namespace

/* Gepard types tests */

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
