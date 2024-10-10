//
// Created by Yile Hu on 10/7/24.
//

#ifndef MYSTL__HEAP_H
#define MYSTL__HEAP_H
#include "../Vector.h"
#include <cassert>
namespace mystl{
    namespace internal{
        template<class T, class Container=mystl::Vector<T>, class Compare=std::less<T>>
        class _heap{
            using storage_type = Container;
            using size_type = size_t;

        private:
            Container storage;
            Compare m_comp;
            void _bubble_up(size_type index){
                while(index != 0 && m_comp(storage[index], storage[_parent(index)])){
                    std::swap(storage[index], storage[_parent(index)]);
                    index = _parent(index);
                }
            };
            void _bubble_down(size_type index){
                while((!_is_leaf(index) && !m_comp(storage[index],storage[_left(index)]))
                || (_has_right(index) && !m_comp(storage[index], storage[_right(index)]))){
                    size_type max_ind = _left(index);
                    max_ind = _has_right(index) ?
                              (m_comp(storage[_left(index)], storage[_right(index)]) ? _left(index) : _right(index))
                            :max_ind;
                    std::swap(storage[index], storage[max_ind]);
                    index = max_ind;
                }
            };
            size_type _parent(size_type index){
                assert(index != 0);
                return (index - 1) / 2;
            };
            size_type _left(size_type index){
                assert(index * 2 + 1 < storage.size());
                return index * 2 + 1;
            }
            size_type _right(size_type index){
                assert(index * 2 + 2 < storage.size());
                return index * 2 + 2;
            }
            bool _is_leaf(size_type index){
                return index * 2 + 1 >= storage.size();
            }
            bool _has_right(size_type index){
                return index * 2 + 2 < storage.size();
            }
        public:
            _heap()=default;
            _heap(size_type n): storage(n){};
            void _insert(T val){
                storage.push_back(val);
                _bubble_up(storage.size() - 1);
            }
            void _pop(){
                std::swap(*storage.begin(), *storage.rbegin());
                storage.pop_back();
                _bubble_down(0);
            }
            T _top(){
                return *storage.begin();
            }
            size_type _size(){
                return storage.size();
            }
            storage_type _storage(){
                return storage;
            }
        };
    }
}
#endif //MYSTL__HEAP_H
