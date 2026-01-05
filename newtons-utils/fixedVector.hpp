#pragma once

#include <cstddef>
#include <stdlib.h>
#include <cstring>

namespace nwt
{
    // Class for fixed size heap allocated arrays
    template<typename T>
    class FixedVector {

        struct Iterator {
        private:
            T* _ptr;

        public:
            Iterator(T* ptr)
                : _ptr(ptr) {
            }

            Iterator& operator++() {
                _ptr++;
                return *this;
            }

            Iterator operator++(int) {
                Iterator itr = *this;
                ++(*this);
                return itr;
            }

            Iterator& operator--() {
                _ptr--;
                return *this;
            }

            Iterator operator--(int) {
                Iterator itr = *this;
                --(*this);
                return itr;
            }

            T& operator[](size_t index) {
                return *(_ptr + index);
            }

            T* operator->() {
                return _ptr;
            }

            T& operator*() {
                return *_ptr;
            }

            bool operator==(const Iterator& other) const {
                return _ptr == other._ptr;
            }

            bool operator!=(const Iterator& other) const {
                return !(*this == other);
            }
        };

        T* _data;
        size_t _size;

    public:
        FixedVector(size_t size)
            : _size(size), _data(size == 0 ? nullptr : new T[size]) {
        }

        FixedVector()
            : _size(0), _data(nullptr) {
        }

        FixedVector(const FixedVector<T>& other);
        FixedVector(FixedVector<T>&& other) noexcept;
        virtual ~FixedVector();


        size_t size() const;
        // void resize(size_t size);
        T* data() const;

        FixedVector<T>& operator=(const FixedVector& other);
        FixedVector<T>& operator=(FixedVector&& other) noexcept;
        constexpr T& operator[](size_t n);
        constexpr const T& operator[](size_t n) const;


        Iterator begin() const;
        Iterator end() const;

    };

    template<typename T>
    inline FixedVector<T>::FixedVector(const FixedVector<T>& other)
        : _size(other._size) {
        _data = new T[other._size];
        std::copy(other._data, other._data + other._size, _data);
    }

    template<typename T>
    inline FixedVector<T>::FixedVector(FixedVector<T>&& other) noexcept
        : _size(other._size), _data(other._data) {

        other._data = nullptr;
        other._size = 0;
    }

    template<typename T>
    inline FixedVector<T>::~FixedVector() {
        delete[] _data;
    }

    template<typename T>
    inline T* FixedVector<T>::data() const {
        return _data;
    }

    template<typename T>
    inline size_t FixedVector<T>::size() const {
        return _size;
    }

    // template<typename T>
    // inline void FixedVector<T>::resize(size_t size){

    // }

    template<typename T>
    inline FixedVector<T>& FixedVector<T>::operator=(const FixedVector<T>& other) {
        if (this == &other) {
            return *this;
        }

        delete[] _data;
        _size = other.size();

        _data = new T[other._size];
        std::copy(other._data, other._data + other._size, _data);

        return *this;
    }

    template<typename T>
    inline FixedVector<T>& FixedVector<T>::operator=(FixedVector<T>&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        delete[] _data;
        _size = other.size();
        _data = other._data;

        other._size = 0;
        other._data = nullptr;

        return *this;
    }


    template<typename T>
    inline constexpr T& FixedVector<T>::operator[](size_t n) {
#ifdef DEBUG
        if (n >= _size) {
            throw std::runtime_error("Index out of range");
    }
#endif

        return _data[n];
}

    template<typename T>
    inline constexpr const T& FixedVector<T>::operator[](size_t n) const {
#ifdef DEBUG
        if (n >= _size) {
            throw std::runtime_error("Index out of range");
    }
#endif

        return _data[n];
    }

    template<typename T>
    inline FixedVector<T>::Iterator FixedVector<T>::begin() const {
        return Iterator(_data);
    }

    template<typename T>
    inline FixedVector<T>::Iterator FixedVector<T>::end() const {
        return Iterator(_data + _size);
    }

} // namespace nwt
