//
// Created by Yile Hu on 9/16/24.
//

#ifndef MYSTL_MY_SHARED_PTR_H
#define MYSTL_MY_SHARED_PTR_H
#include <atomic>

template <class T>
struct SpControlBlock{
    T *m_data;
    std::atomic<int> m_refcnt;

    void incref(){
        m_refcnt.fetch_add(1, std::memory_order_relaxed);
    }

    void decref(){
        if(m_refcnt.fetch_sub(1, std::memory_order_relaxed) == 1){
            delete this;
        }
    }

    long cntref() const noexcept{
        return m_refcnt.load(std::memory_order_relaxed);
    }

    explicit SpControlBlock(T *ptr) : m_refcnt(1), m_data(ptr){}
    SpControlBlock(SpControlBlock &&)=delete;
    ~SpControlBlock(){
        delete m_data;
    }
};
template<class T>
struct my_shared_ptr{
    SpControlBlock<T> *m_cb;
    explicit my_shared_ptr(std::nullptr_t = nullptr) noexcept : m_cb(nullptr){};

    template<class Deleter>
    explicit my_shared_ptr(T *ptr) : m_cb(new SpControlBlock<T>(ptr)){};

    my_shared_ptr(my_shared_ptr const &that){
        m_cb = that.m_cb;
        m_cb->incref();
    }

    T* get(){
        return m_cb->m_data;
    }

    T& operator*(){
        return *(m_cb->m_data);
    }

    ~my_shared_ptr(){
        m_cb->decref();
    }
};
//template<class T, class ...Args>
//my_shared_ptr<T> makeShared(Args &&...args){
//    return my_shared_ptr<T>(new T(std::forward<Args>(args)...));
//}

template <class T>
struct my_weak_ptr{

};

template<class T>
struct EnableSharedFromThis{

};
#endif //MYSTL_MY_SHARED_PTR_H
