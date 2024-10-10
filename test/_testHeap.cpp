//
// Created by Yile Hu on 10/8/24.
//
#include <gtest/gtest.h>
#include "../include/internal/_heap.h"
#include <vector>
TEST(HeapTest, Insert){
    mystl::internal::_heap<int, mystl::Vector<int>> pq;
    for(int i = 0;i<10;i++){
        pq._insert(i);
    }
    EXPECT_EQ(pq._size(), 10);
}
TEST(HeapTest, TopAndPop){
    mystl::internal::_heap<int, std::vector<int>> pq;
    for(int i = 0;i<10;i++){
        pq._insert(i);
    }
    for(int i = 0;i<10;i++){
        EXPECT_EQ(pq._top(), i);
        pq._pop();
    }
}

TEST(HeapTest, Comparator){
    mystl::internal::_heap<int, std::vector<int>, std::greater<>> pq;
    for(int i = 0;i<10;i++){
        pq._insert(i);
    }
    for(int i = 9;i>=0;i--){
        EXPECT_EQ(pq._top(), i);
        pq._pop();
    }
}