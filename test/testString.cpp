//
// Created by Yile Hu on 9/19/24.
//

#include <iostream>
#include <vector>
#include <fcntl.h>
#include <cassert>
#include <algorithm>
#include "../include/String.h"
#include "../include/Vector.h"
#include <gtest/gtest.h>

TEST(StringTest, Size) {
mystl::String v;
EXPECT_EQ(v.size(), 0);
}