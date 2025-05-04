#pragma once

#include <cstddef>
#include <stdlib.h>
#include <cstring>

namespace nwt
{
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
                Iterator itr = *this;
                _ptr += index;
                return *_ptr;
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
            : _size(size), _data(new T[size]) {
        }

        FixedVector()
            : _size(0), _data(nullptr) {
        }

        FixedVector(const FixedVector<T>& other);
        FixedVector(FixedVector<T>&& other) noexcept;
        virtual ~FixedVector();

        FixedVector<T>& operator=(const FixedVector& other);

        size_t size() const;

        constexpr T& operator[](size_t n);
        constexpr const T& operator[](size_t n) const;

        Iterator begin() {
            return Iterator(_data);
        }

        Iterator end() {
            return Iterator(_data + _size);
        }

    };

    template<typename T>
    inline FixedVector<T>::FixedVector(const FixedVector<T>& other)
        : _size(other._size) {
        _data = reinterpret_cast<T*>(std::malloc(other._size));
        std::memcpy(_data, other._data, other._size);
    }

    template<typename T>
    inline FixedVector<T>::FixedVector(FixedVector<T>&& other) noexcept
        : _size(other._size) {
        _data = other._data;

        other._data = nullptr;
        other._size = 0;
    }

    template<typename T>
    inline FixedVector<T>::~FixedVector() {
        delete[] _data;
    }

    template<typename T>
    inline FixedVector<T>& FixedVector<T>::operator=(const FixedVector<T>& other) {
        if (this == &other) {
            return *this;
        }

        delete[] _data;
        _size = other.size();

        _data = reinterpret_cast<T*>(std::malloc(other.size()));
        std::memcpy(_data, other._data, other.size());

        return *this;
    }


    template<typename T>
    inline size_t FixedVector<T>::size() const {
        return _size;
    }

    template<typename T>
    inline constexpr T& FixedVector<T>::operator[](size_t n) {
        return _data[n];
    }

    template<typename T>
    inline constexpr const T& FixedVector<T>::operator[](size_t n) const {
        return _data[n];
    }

} // namespace nwt
