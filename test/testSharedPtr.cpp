//
// Created by Yile Hu on 10/1/24.
//
#include "../include/SharedPtr.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
TEST(SharedPtrTest, IncRef) {
    int* i = new int;
    mystl::SharedPtr<int> ptr(i);
    EXPECT_EQ(ptr.use_count(), 1);
    mystl::SharedPtr<int> ptr2 = ptr;
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
    mystl::SharedPtr<int> ptr3 = ptr;
    EXPECT_EQ(ptr.use_count(), 3);
    EXPECT_EQ(ptr2.use_count(), 3);
    EXPECT_EQ(ptr3.use_count(), 3);
}

TEST(SharedPtrTest, DecRef) {
    int* i = new int;
    mystl::SharedPtr<int> ptr(i);
    EXPECT_EQ(ptr.use_count(), 1);
    {
        mystl::SharedPtr<int> ptr2 = ptr;
        EXPECT_EQ(ptr.use_count(), 2);
        EXPECT_EQ(ptr2.use_count(), 2);
    }
    EXPECT_EQ(ptr.use_count(), 1);
}

TEST(SharedPtrTest, Access) {
    int* i = new int;
    mystl::SharedPtr<int> ptr(i);
    EXPECT_EQ(ptr.use_count(), 1);
    mystl::SharedPtr<int> ptr2 = ptr;
    *ptr = 100;
    EXPECT_EQ(*i, 100);
    EXPECT_EQ(*ptr2, 100);
    *ptr2 = 200;
    EXPECT_EQ(*ptr, 200);
}

TEST(SharedPtrTest, Casting){

}

TEST(SharedPtrTest, MakeShared){
    //std::shared_ptr ptr = std::make_shared<std::vector<int>>(3, 100);
    mystl::SharedPtr ptr = mystl::make_shared<std::vector<int>>(3, 100);
    EXPECT_EQ((*ptr.get())[0], 100);
    EXPECT_EQ((*ptr.get())[1], 100);
    EXPECT_EQ((*ptr.get())[2], 100);
    EXPECT_EQ((*ptr.get()).size(), 3);
}
TEST(SharedPtrTest, MakeSharedArray){
    std::shared_ptr ptr = std::make_shared<int[]>(3, 10);
    EXPECT_EQ(ptr.get()[0], 10);
    EXPECT_EQ(ptr.get()[1], 10);
    EXPECT_EQ(ptr.get()[2], 10);
}