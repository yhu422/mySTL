//
// Created by Yile Hu on 9/19/24.
//

#ifndef MYSTL_STRING_H
#define MYSTL_STRING_H
#pragma once
#include <cstddef>
#include <iterator>
#include <memory>
#include <limits>
#include <stdexcept>
#include <utility>
#include <iostream>
namespace mystl {
    template<class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT>>
    class BasicString {
    public:

        using size_type = size_t;

        const static size_type npos = -1;

        BasicString() noexcept(noexcept(Allocator())): BasicString(Allocator()) {}

        explicit BasicString(const Allocator &alloc) : m_data(nullptr), m_size(0), m_cap(0), m_alloc(alloc) {}

        BasicString(size_type count, CharT ch, const Allocator &alloc = Allocator()) : m_alloc(alloc) {
            m_cap = count + 1;
            m_size = count;
            m_data = m_alloc.allocate(m_cap);
            for (size_type i = 0; i < m_size; i++) {
                std::construct_at(&m_data[i], ch);
            }
            std::construct_at(&m_data[m_size], CharT());
        };

        BasicString(const BasicString &other, size_type pos, const Allocator &alloc = Allocator()) : m_alloc(
                alloc) {
            m_cap = other.m_size - pos + 1;
            m_size = other.m_size - pos;
            m_data = m_alloc.allocate(m_cap);
            for (size_type i = 0; i < m_size; i++) {
                std::construct_at(&m_data[i], other[pos + i]);
            }
            std::construct_at(&m_data[m_size], CharT());
        }

        CharT &at(size_t index) {
            if (index >= m_size) throw std::out_of_range("BasicString::at");
            return m_data[index];
        }

        const CharT &at(size_t index) const {
            if (index >= m_size) throw std::out_of_range("BasicString::at");
            return m_data[index];
        }

        CharT &operator[](size_t index) {
            return m_data[index];
        }

        CharT &front() {
            return m_data[0];
        }

        const CharT &front() const {
            return m_data[0];
        }

        CharT &back() {
            return m_data[m_size - 1];
        }

        const CharT &back() const {
            return m_data[m_size - 1];
        }

        const CharT *data() const noexcept {
            return m_data;
        }

        CharT *data() noexcept {
            return m_data;
        }

        size_t size() const {
            return m_size;
        }

        const CharT *c_str() const {
            return m_data;
        }

        ~BasicString() {
            for (size_type i = 0; i <= m_size; i++) {
                std::destroy_at(&m_data[i]);
            }
            m_alloc.deallocate(m_data, m_cap);
        }

        void reserve(size_type new_cap) {
            if (m_cap >= new_cap) return;

        }

        BasicString &append(const BasicString &str) {
            for (size_type i = 0; i < str.m_size; i++) {
                std::construct_at(&m_data[m_size + i], str.m_data[i]);
            }
            m_size += str.m_size;
            std::construct_at(&m_data[m_size], CharT());
            return *this;
        }

        BasicString<CharT, Traits, Allocator> operator+(const BasicString<CharT, Traits, Allocator> &rhs) {

        }
    private:
        CharT *m_data;
        size_t m_size;
        size_t m_cap;
        Allocator m_alloc;
    };
    using String = BasicString<char>;
}
#endif //MYSTL_STRING_H
