//
// Created by Yile Hu on 9/16/24.
//

#ifndef MYSTL_MY_SHARED_PTR_H
#define MYSTL_MY_SHARED_PTR_H
#include <atomic>
#include <memory>
template <class T, class Deleter =std::default_delete<T>>
struct DataBlock{
    T* m_data;
    std::atomic<long> m_refcnt;
    Deleter m_delete;
    DataBlock() : m_data(nullptr), m_refcnt(1){}
    DataBlock(T* ptr) : m_data(ptr), m_refcnt(1){}
    DataBlock(T* ptr, Deleter d) : m_data(ptr), m_refcnt(1), m_delete(d){}
    void inc_ref(){
        m_refcnt.fetch_add(1);
    }
    void dec_ref(){
        if(m_refcnt.fetch_sub(1) == 1){
            delete this;
        }
    }

    long get_ref(){
        return m_refcnt.load();
    }

    ~DataBlock(){
        m_delete(m_data);
    }
};
template<class T>
struct my_shared_ptr{
    T* m_ptr;
    DataBlock<T> *m_cb;
    explicit my_shared_ptr(std::nullptr_t = nullptr) noexcept : m_cb(nullptr){};

    template<class Y> requires(std::is_convertible_v<Y*, T*>)
    explicit my_shared_ptr(Y* ptr) :m_ptr(ptr), m_cb(new DataBlock<T>(ptr)){};

    template<class Y, class Deleter> requires(std::is_convertible_v<Y*, T*>)
    explicit my_shared_ptr(Y* ptr, Deleter d) : m_ptr(ptr), m_cb(new DataBlock<T, Deleter>(ptr, std::move(d))){};

    my_shared_ptr( const my_shared_ptr& that ) noexcept{
        m_ptr = that.m_ptr;
        m_cb = that.m_cb;
        if(m_cb) {
            m_cb->inc_ref();
        }
    }

    my_shared_ptr(my_shared_ptr&& that) noexcept{
        m_ptr = that.m_ptr;
        m_cb = that.m_cb;
        that.m_cb = nullptr;
        that.m_ptr = nullptr;
    }

    my_shared_ptr& operator=(const my_shared_ptr& that){
        m_ptr = that.m_ptr;
        m_cb = that.m_cb;
        if(m_cb){
            m_cb->inc_ref();
        }
        return *this;
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
