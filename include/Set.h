//
// Created by Yile Hu on 10/1/24.
//

#ifndef MYSTL_SET_H
#define MYSTL_SET_H
#include <algorithm>
#include <memory>
#include "internal/_rbTree.h"
namespace mystl {
    template<
            class Key,
            class Compare = std::less <Key>,
            class Allocator = std::allocator <Key>
    >
    class Set{
        using iterator = _rbTree<Key>::iterator;
        using const_iterator = const _rbTree<Key>::iterator;
        using value_type = Key;
    private:
        _rbTree<Key> rbTree;
        size_t m_size;
        Compare m_comp;
        Allocator m_alloc;
    public:
        Set():m_size(0){};
        explicit Set( const Compare& comp,
                      const Allocator& alloc = Allocator() ):m_size(0),m_comp(comp), m_alloc(alloc){};

        size_t size(){
            return m_size;
        }

        std::pair<iterator, bool> insert( const value_type& value ){
            ++m_size;
            return rbTree.insert(value);
        }
        std::pair<iterator, bool> insert( value_type&& value ){
            ++m_size;
            return rbTree.insert(std::move(value));
        }

        iterator find( const Key& key ){
            return rbTree.find(key);
        }

        const_iterator find( const Key& key ) const{
            return rbTree.find(key);
        }

        bool contains( const Key& key ) const{
            return *find(key) != nullptr;
        }

        iterator begin(){
            return rbTree.begin();
        }
        iterator end(){
            return rbTree.end();
        }
    };
}

#endif //MYSTL_SET_H
