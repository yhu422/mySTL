#include "../include/Vector.h"
#include <algorithm>
#include <gtest/gtest.h>

TEST(VectorTest, Size) {
    mystl::Vector<int> v;
    EXPECT_EQ(v.size(), 0);
    v.push_back(1);
    EXPECT_EQ(v.size(), 1);
    v.resize(10,20);
    EXPECT_EQ(v.size(), 10) << "resize() changes the size to the exact number";
    v.reserve(100);
    EXPECT_EQ(v.size(), 10) << "reserving a larger capacity should not change size";
}

TEST(VectorTest, PushBack){
    mystl::Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    EXPECT_EQ(v, mystl::Vector({1,2,3}));
}

TEST(VectorTest, Capacity){
    mystl::Vector<int> v;
    v.reserve(100);
    EXPECT_GE(v.capacity(), 100);
}

TEST(VectorTest, Insert){
    mystl::Vector<int> v;
    v.insert(v.cbegin(), 100);
    v.insert(v.cbegin(), 99);
    v.insert(v.cbegin(), 98);
    EXPECT_EQ(v, mystl::Vector({98,99,100}));
    v.insert(v.cend(), 101);
    v.insert(v.cend(), 102);
    v.insert(v.cend(), 103);
    EXPECT_EQ(v, mystl::Vector({98,99,100,101,102,103}));
    mystl::Vector<int> tmp{999,998,997};
    v.insert(v.begin(), tmp.cbegin(), tmp.cend());
    EXPECT_EQ(v, mystl::Vector({999,998,997,98,99,100,101,102,103}));
    mystl::Vector<int> v2;
    v2.insert(v2.cbegin(), 5,10);
    EXPECT_EQ(v2, mystl::Vector({10,10,10,10,10}));
    v2.insert(v2.cbegin() + 1, 3, 5);
    EXPECT_EQ(v2, mystl::Vector({10,5,5,5,10,10,10,10}));
    v2.insert(v2.cend(), 2, 99);
    EXPECT_EQ(v2, mystl::Vector({10,5,5,5,10,10,10,10, 99, 99}));
    v2.insert(v2.cbegin() + 3, {7,9,11});
    EXPECT_EQ(v2, mystl::Vector({10,5,5,7,9,11,5,10,10,10,10, 99, 99}));
}

TEST(VectorTest, PopBack){
    mystl::Vector<int> v{1,2,3};
    v.pop_back();
    EXPECT_EQ(v, mystl::Vector<int>({1,2}));
    v.pop_back();
    EXPECT_EQ(v, mystl::Vector<int>({1}));
    v.pop_back();
    EXPECT_EQ(v, mystl::Vector<int>());
}

TEST(VectorTest, Clear){
    mystl::Vector<int> v{1,2,3,4,5};
    v.reserve(50);
    v.clear();
    EXPECT_EQ(v.size(), 0);
    EXPECT_GE(v.capacity(), 50);
}

TEST(VectorTest, ShrinkToFit){
    mystl::Vector<int> v{1,2,3,4,5};
    v.reserve(50);
    v.pop_back();
    v.pop_back();
    v.shrink_to_fit();
    EXPECT_EQ(v, mystl::Vector<int>({1,2,3}));
    EXPECT_EQ(v.capacity(), 3);
}

TEST(VectorTest, Access){
    mystl::Vector<int> v{0,1,2,3,4};
    for(size_t i = 0;i<v.size();i++){
        EXPECT_EQ(v[i], i);
    }
}