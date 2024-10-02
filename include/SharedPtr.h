//
// Created by Yile Hu on 9/16/24.
//

#ifndef MYSTL_MY_SHARED_PTR_H
#define MYSTL_MY_SHARED_PTR_H
#include <atomic>
#include <memory>

namespace mystl {
    template<class T, class Deleter =std::default_delete<T>>
    struct DataBlock {
        std::atomic<long> m_refcnt;
        Deleter m_delete;
        T *m_data;

        DataBlock() : m_data(nullptr), m_refcnt(1) {}
        explicit DataBlock(T *ptr) : m_data(ptr), m_refcnt(1) {}
        DataBlock(T *ptr, Deleter d) : m_data(ptr), m_refcnt(1), m_delete(d) {}

        void inc_ref() { m_refcnt.fetch_add(1); }
        void dec_ref() {
            if (m_refcnt.fetch_sub(1) == 1) {
                delete this;
            }
        }
        long get_ref() { return m_refcnt.load(); }
        ~DataBlock() { m_delete(m_data); }
    };

    template<class T>
    class SharedPtr {
    private:
        T *m_ptr;
        DataBlock<T> *m_cb;
    public:
        explicit SharedPtr(std::nullptr_t = nullptr) noexcept: m_cb(nullptr) {};

        template<class Y>
        requires(std::is_convertible_v<Y *, T *>)
        explicit SharedPtr(Y *ptr) :m_ptr(ptr), m_cb(new DataBlock<T>(ptr)) {};

        template<class Y, class Deleter>
        requires(std::is_convertible_v<Y *, T *>)
        SharedPtr(Y *ptr, Deleter d) : m_ptr(ptr), m_cb(new DataBlock<Y, Deleter>(ptr, std::move(d))) {};

        template< class Deleter >
        SharedPtr( std::nullptr_t ptr, Deleter d ): m_ptr(ptr), m_cb(new DataBlock<T, Deleter>(ptr, std::move(d))){};

        template< class Y, class Deleter, class Alloc >
        SharedPtr( Y* ptr, Deleter d, Alloc alloc ): m_ptr(ptr), m_cb(){};

        SharedPtr(const SharedPtr &that) noexcept {
            m_ptr = that.m_ptr;
            m_cb = that.m_cb;
            if (m_cb) {
                m_cb->inc_ref();
            }
        }

        SharedPtr(SharedPtr &&that) noexcept {
            m_ptr = that.m_ptr;
            m_cb = that.m_cb;
            that.m_cb = nullptr;
            that.m_ptr = nullptr;
        }

        SharedPtr &operator=(const SharedPtr &that) {
            m_ptr = that.m_ptr;
            m_cb = that.m_cb;
            if (m_cb) {
                m_cb->inc_ref();
            }
            return *this;
        }

        T *get() {return m_cb->m_data;}
        T &operator*() {return *(m_cb->m_data);}
        [[nodiscard]] long use_count() const noexcept{return m_cb->get_ref();}

        ~SharedPtr() { if(m_cb) m_cb->dec_ref(); }
    };



}
#endif //MYSTL_MY_SHARED_PTR_H
