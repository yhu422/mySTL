//
// Created by Yile Hu on 9/11/24.
//

#ifndef MYSTL_VECTOR_H
#define MYSTL_VECTOR_H


#pragma once
#include <cstddef>
#include <iterator>
#include <memory>
#include <limits>
#include <stdexcept>
#include <utility>
#include <iostream>
namespace mystl {
    template<class T, class Allocator = std::allocator<T>>
    struct Vector {
        //using <type1> = <type2> makes type1 refer to the same type as type2. Purely for readability
        using value_type = T;
        using allocator_type = Allocator;
        using allocator_traits = std::allocator_traits<Allocator>;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = std::allocator_traits<Allocator>::const_pointer;
        using const_pointer = std::allocator_traits<Allocator>::pointer;

        //Implementing as a dynamically sized array, so this is just a pointer to T;
        using iterator = T *;
        using const_iterator = const T *;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        [[no_unique_address]] Allocator m_alloc;
        T *m_data;
        size_t m_size;
        size_t m_cap;

        //Constructor of Vector is noexcept if Allocator() is noexcept
        Vector() noexcept(noexcept(Allocator())) {
            m_data = nullptr;
            m_size = 0;
            m_cap = 0;
        }

        explicit Vector(const Allocator &alloc) noexcept {
            m_data = nullptr;
            m_size = 0;
            m_cap = 0;
            m_alloc = alloc;
        }

        explicit Vector(size_type count, const Allocator &alloc = Allocator()) : m_alloc(alloc) {
            m_size = count;
            m_cap = count;
            m_data = alloc.allocate(count);
            for (size_type i = 0; i != m_size; i++) {
                std::construct_at(&m_data[i]);
            }
        }

        Vector(size_type count, const T &value, const Allocator &alloc = Allocator()) : m_alloc(alloc) {
            m_size = count;
            m_cap = count;
            m_data = m_alloc.allocate(count);
            for (size_type i = 0; i != m_size; i++) {
                std::construct_at(&m_data[i], value);
            }
        }

        Vector(const Vector &other) {
            m_size = other.m_size;
            m_cap = other.m_cap;
            m_data = nullptr;
            if (other.m_data) {
                m_data = m_alloc.allocate(other.m_cap);
                for (size_type i = 0; i < other.m_size; i++) {
                    std::construct_at(&m_data[i], std::as_const(other.m_data[i]));
                }
            }
        }

        Vector(const Vector &other, const Allocator &alloc) : m_alloc(alloc) {
            m_size = other.m_size;
            m_cap = other.m_cap;
            m_data = nullptr;
            if (other.m_data) {
                m_data = m_alloc.allocate(other.m_cap);
                for (size_type i = 0; i < other.m_size; i++) {
                    std::construct_at(&m_data[i], std::as_const(other.m_data[i]));
                }
            }
        }

        Vector(Vector &&other) noexcept: m_alloc(std::move(other.m_alloc)) {
            m_size = other.m_size;
            m_cap = other, m_cap;
            m_data = other.m_data;
            other.m_data = nullptr;
            other.m_cap = 0;
            other.m_size = 0;
        }

        Vector(Vector &&other, const Allocator &alloc) {
            m_alloc = alloc;
            m_size = other.m_size;
            m_cap = other.m_cap;
            if (other.m_alloc != m_alloc) [[unlikely]] {
                m_data = m_alloc.allocate(m_cap);
                for (size_type i = 0; i < m_size; i++) {
                    std::construct_at(&m_data[i], std::move_if_noexcept(m_data[i]));
                }
            } else {
                m_data = other.m_data;
                other.m_data = nullptr;
                other.m_cap = 0;
                other.m_size = 0;
            }
        }

        template<std::input_iterator InputIt>
        Vector(InputIt first, InputIt last, const Allocator &alloc = Allocator()) : m_alloc(alloc) {
            size_t s = std::distance(first, last);
            m_size = s;
            m_cap = s;
            m_data = m_alloc.allocate(m_cap);
            for (size_type i = 0; i < m_size; i++) {
                std::construct_at(&m_data[i], *first);
                ++first;
            }
        }

        Vector(std::initializer_list<T> init, const Allocator &alloc = Allocator())
                : Vector(init.begin(), init.end(), alloc) {}

        template<class U, class Al>
        const bool _check_propagation(const Vector<T, Al> vec) {
            return std::allocator_traits<Al>::propagate_on_container_copy_assignment::valuel;
        }

        Vector &operator=(const Vector &other) {
            if (&other == this)[[unlikely]] return *this;
            for (size_type i = 0; i < m_size; i++) {
                std::destroy_at(&m_data[i]);
            }
            reserve(other.m_cap);
            for (size_type i = 0; i < other.m_size; i++) {
                m_data[i] = other.m_data[i];
                //std::construct_at(&m_data[i], std::as_const(other.m_data[i]));
            }
            m_size = other.m_size;
            return *this;
//        if(allocator_traits::propagate_on_container_copy_assignment::value){
//            if(m_alloc != other.m_alloc)
//        }
        }

        Vector &operator=(Vector &&other) noexcept {
            if (&other == this) [[unlikely]] return *this;
            for (size_type i = 0; i < m_size; i++) {
                std::destroy_at(&m_data[i]);
            }
            m_alloc.deallocate(m_data, m_cap);
            m_data = other.m_data;
            m_size = other.m_size;
            m_cap = other.m_cap;
            other.m_data = nullptr;
            other.m_size = 0;
            other.m_cap = 0;
            return *this;
        }


        [[nodiscard]]  bool empty() const {
            return m_size == 0;
        }

        [[nodiscard]]  size_type size() const {
            return m_size;
        }

        [[nodiscard]]  size_type capacity() const {
            return m_cap;
        }

        //We cannot use memcpy() for reallocation.
        //For example, consider an object with a pointer to itself as a member variable and handles the invariant
        //through move and copy constructor/assignment
        void reserve(size_type new_cap) {
            if (new_cap > capacity()) {
                new_cap = std::max(m_cap * 2, new_cap);
                T *old_data = m_data;
                m_data = m_alloc.allocate(new_cap);
                if (old_data) {
                    for (size_type i = 0; i != m_size; i++) {
                        std::construct_at(&m_data[i], std::move_if_noexcept(old_data[i]));
                    }
                    for (size_type i = 0; i != m_size; i++) {
                        std::destroy_at(&old_data[i]);
                    }
                    m_alloc.deallocate(old_data, m_cap);
                }
                m_cap = new_cap;
            }
        }

        void shrink_to_fit() {
            T *old_data = m_data;
            if (m_size == 0) {
                if (old_data) {
                    m_alloc.deallocate(old_data, m_cap);
                }
            } else {
                m_data = m_alloc.allocate(m_size);
                if (old_data) {
                    for (size_type i = 0; i != m_size; i++) {
                        std::construct_at(&m_data[i], std::move_if_noexcept(old_data[i]));
                    }
                    for (size_type i = 0; i != m_size; i++) {
                        std::destroy_at(&old_data[i]);
                    }
                    m_alloc.deallocate(old_data, m_cap);
                }
            }
            m_cap = m_size;
        }

        void clear() {
            for (size_type i = 0; i < m_size; i++) {
                std::destroy_at(&m_data[i]);
            }
            m_size = 0;
        }

        iterator insert(const_iterator pos, const T &value) {
            size_t j = pos - m_data;
            reserve(m_size + 1);
            for (size_t i = m_size; i > j; i--) {
                std::construct_at(&m_data[i], std::move(m_data[i-1]));
                std::destroy_at(&m_data[i-1]);
            }
            std::construct_at(&m_data[j], value);
            m_size += 1;
            return &m_data[j];
        }

        iterator insert(const_iterator pos, T &&value) {
            size_t j = pos - m_data;
            reserve(m_size + 1);
            for (size_t i = m_size; i > j; i--) {
                std::construct_at(&m_data[i], std::move(m_data[i-1]));
                std::destroy_at(&m_data[i-1]);
            }
            std::construct_at(&m_data[j], std::move(value));
            m_size += 1;
            return &m_data[j];
        }

        iterator insert(const_iterator pos, size_type count, const T &value) {
            size_t j = pos - m_data;
            reserve(m_size + count);
            for (size_t i = m_size + count - 1; i >= j + count; i--) {
                std::construct_at(&m_data[i], std::move(m_data[i-count]));
                std::destroy_at(&m_data[i-count]);
            }
            for (size_t i = 0; i < count; i++) {
                std::construct_at(&m_data[j + i], value);
            }
            m_size += count;
            return &m_data[j];
        }

        template<std::input_iterator InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last) {
            size_t count = last - first;
            size_t j = pos - m_data;
            reserve(m_size + count);
            for (size_t i = m_size + count - 1; i >= j + count; i--) {
                std::construct_at(&m_data[i], std::move(m_data[i-count]));
                std::destroy_at(&m_data[i-count]);
            }
            for (size_t i = 0; i < count; i++) {
                std::construct_at(&m_data[j + i], *first);
                first++;
            }
            m_size += count;
            return &m_data[j];
        }

        iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
            return insert(pos, ilist.begin(), ilist.end());
        }

        template<class... Args>
        iterator emplace(const_iterator pos, Args &&... args) {
            size_t j = pos - m_data;
            reserve(m_size + 1);
            for (size_t i = m_size - 1; i >= j; i--) {
                std::construct_at(&m_data[i + 1], std::move(m_data[i]));
                std::destroy_at(&m_data[i]);
            }
            std::construct_at(&m_data[j], std::forward<Args>(args)...);
            m_size += 1;
            return &m_data[j];
        }

        iterator erase(const_iterator pos) {
            size_t j = pos - m_data;
            std::destroy_at(pos);
            for (size_t i = j + 1; i < m_size; i++) {
                std::construct_at(&m_data[i - 1], m_data[i]);
                std::destroy_at(&m_data[i]);
            }
            m_size -= 1;
            return m_data + j;
        }

        iterator erase(const_iterator first, const_iterator last) {
            size_type count = last - first;
            size_type start_pos = last - m_data;
            for (size_t i = start_pos; i < m_size; i++) {
                m_data[start_pos - count] = std::move(m_data[i]);
            }
            m_size -= count;
            for (size_t i = m_size; i < m_size + count; i++) {
                std::destroy_at(&m_data[i]);
            }
            return const_cast<iterator>(first);
        }

        void push_back(const T &value) {
            reserve(m_size + 1);
            std::construct_at(&m_data[m_size], value);
            m_size += 1;
        }

        void push_back(T &&value) {
            reserve(m_size + 1);
            std::construct_at(&m_data[m_size], std::move(value));
            m_size += 1;
        }

        template<class... Args>
        reference emplace_back(Args &&... args) {
            reserve(m_size + 1);
            std::construct_at(&m_data[m_size], std::forward<Args>(args)...);
            m_size += 1;
            return m_data[m_size - 1];
        }

        void pop_back() {
            std::destroy_at(&m_data[m_size - 1]);
            m_size -= 1;
        }

        void resize(size_type count) {
            if (count == size()) [[unlikely]] return;
            if (count > size()) {
                reserve(count);
                for (size_type i = m_size; i < count; i++) {
                    std::construct_at(&m_data[i]);
                }
                m_size = count;
            } else {
                for (size_type i = count; i < m_size; i++) {
                    std::destroy_at(&m_data[i]);
                }
                m_size = count;
            }
        }

        void resize(size_type count, const value_type &value) {
            if (count == size()) [[unlikely]] return;
            if (count > size()) {
                reserve(count);
                for (size_type i = m_size; i < count; i++) {
                    std::construct_at(&m_data[i], value);
                }
                m_size = count;
            } else {
                for (size_type i = count; i < m_size; i++) {
                    std::destroy_at(&m_data[i]);
                }
                m_size = count;
            }
        }

        void swap(Vector &other) noexcept {
            std::swap(m_size, other.m_size);
            std::swap(m_cap, other.m_cap);
            std::swap(m_data, other.m_data);
            std::swap(m_alloc, other.m_alloc);
        }


        reference operator[](size_type pos) {
            return *(m_data + pos);
        }

        const_reference operator[](size_type pos) const {
            return *(m_data + pos);
        }

        reference at(size_type pos) {
            if (pos >= size())[[unlikely]] throw std::out_of_range("Vector::at");
            return *(m_data + pos);
        }

        const_reference at(size_type pos) const {
            if (pos >= size())[[unlikely]] throw std::out_of_range("Vector::at");
            return *(m_data + pos);
        }

        reference front() {
            return m_data[0];
        }

        const_reference front() const {
            return m_data[0];
        }

        reference back() {
            return m_data[m_size - 1];
        }

        const_reference back() const {
            return m_data[m_size - 1];
        }

        T *data() {
            return m_data;
        }

        const T *data() const {
            return m_data;
        }

        //We need two versions of begin for scenario like const vector<int> v; v.begin();
        iterator begin() {
            return m_data;
        }

        const_iterator begin() const {
            return m_data;
        }

        const_iterator cbegin() const {
            return m_data;
        }

        iterator end() noexcept {
            return m_data + size();
        }

        const_iterator end() const noexcept {
            return m_data + size();
        }

        const_iterator cend() const noexcept {
            return m_data + size();
        }

        reverse_iterator rbegin() {
            return std::make_reverse_iterator(m_data + m_size);
        }

        const_reverse_iterator rbegin() const {
            return std::make_reverse_iterator(m_data + m_size);
        }

        const_reverse_iterator crbegin() const noexcept {
            return std::make_reverse_iterator(m_data + m_size);
        }

        reverse_iterator rend() noexcept {
            return std::make_reverse_iterator(m_data);
        }

        const_reverse_iterator rend() const noexcept {
            return std::make_reverse_iterator(m_data);
        }

        const_reverse_iterator crend() const noexcept {
            return std::make_reverse_iterator(m_data);
        }

        allocator_type get_allocator() const {
            return m_alloc;
        }

        ~Vector() {
            for (size_t i = 0; i < m_size; i++) {
                std::destroy_at(&m_data[i]);
            }
            if (m_data) {
                m_alloc.deallocate(m_data, m_cap);
                m_data = nullptr;
            }
        }
    };
    template< class T, class Alloc >
    bool operator==( const mystl::Vector<T, Alloc>& lhs,
                     const mystl::Vector<T, Alloc>& rhs ) {
        if(lhs.size() != rhs.size()) return false;
        return std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }
}



#endif //MYSTL_VECTOR_H
