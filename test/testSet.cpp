//
// Created by Yile Hu on 10/1/24.
//
#include "../include/Set.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <set>
TEST(SetTest, Insert){
    mystl::Set<int> s;
    for(int i = 0;i<10;i++){
        s.insert(i);
    }
    auto it = s.begin();
    int i = 0;
    while(it != s.end()){
        EXPECT_EQ(*it, i);
        ++it;
        ++i;
    }
}