//
// Created by Yile Hu on 10/1/24.
//
#include "../include/internal/_rbTree.h"
#include <algorithm>
#include <gtest/gtest.h>

TEST(RBTreeTest, Insert){
    mystl::_rbTree<int> t;
    for(int i = 0;i<10;i++){
        t.insert(i);
    }
    for(int i = 0;i<10;i++){
        EXPECT_TRUE(*t.find(i) != nullptr);
    }
}
template<class T>
void _check_red(mystl::_rbTreeNode<T>* t){
    if(t->left != nullptr){
        EXPECT_FALSE(t->color == mystl::RED && t->left->color == mystl::RED);
        _check_red(t->left);
    }
    if(t->right != nullptr){
        EXPECT_FALSE(t->color == mystl::RED && t->right->color == mystl::RED);
        _check_red(t->right);
    }
}

TEST(RBTreeTest, RBConstraint){
    mystl::_rbTree<int> t;
    for(int i = 0;i<10;i++){
        t.insert(i);
    }
    EXPECT_TRUE(t.root->color == mystl::BLACK);
    _check_red(t.root);
}
