// Copryright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

/// \file core
/// \author Arthapz

#pragma once

#include <type_traits>
// FROM
// https://stackoverflow.com/questions/8054273/how-to-implement-an-stl-style-iterator-and-avoid-common-pitfalls

/// \module core
namespace storm::core {

    /// An utility class for implementing custom iterator
    template<typename T, class A = std::allocator<T>>
    class Iterator {
    public:
        using value_type      = typename A::value_type;
        using difference_type = typename A::difference_type;
        using reference       = typename A::reference;
        using pointer         = typename A::pointer;
        using size_type       = typename A::size_type;

        Iterator<T, A>(const Iterator<T, A> &);
        ~Iterator<T, A>();
        Iterator<T, A> &operator=(const Iterator<T, A> &);
        Iterator<T, A> &operator++();
        reference operator*() const;
        friend void swap(Iterator<T, A> &lhs, Iterator<T, A> &rhs);
    };

    // An utility class form implementing custom input interator
    template<typename T, class A = std::allocator<T>>
    class Input_iterator : public virtual Iterator<T, A> {
        Iterator<T, A> operator++(int); // postfix increment
        value_type operator*() const;
        pointer operator->() const;
        friend bool operator==(const Iterator<T, A> &, const Iterator<T, A> &);
        friend bool operator!=(const Iterator<T, A> &, const Iterator<T, A> &);
    };

    // An utility class form implementing custom output interator
    template<typename T, class A = std::allocator<T>>
    class Output_iterator : public virtual Iterator<T, A> {
        reference operator*() const;
        Iterator<T, A> operator++(int);
    };

    // An utility class form implementing custom forward interator
    template<typename T, class A = std::allocator<T>>
    class Forward_iterator : public Input_iterator<T, A>, public Output_iterator<T, A> {
        Forward_iterator();
    };

    // An utility class form implementing custom bidirectional interator
    template<typename T, class A = std::allocator<T>>
    class Bidirectional_iterator : public Forward_iterator<T, A> {
        Iterator<T, A> &operator--();
        Iterator<T, A> operator--(int);
    };

    // An utility class form implementing custom random access interator
    template<typename T, class A = std::allocator<T>>
    class random_access_iterator : Bidirectional_iterator<T, A> {
        friend bool operator<(const Iterator<T, A> &, const Iterator<T, A> &);
        friend bool operator>(const Iterator<T, A> &, const Iterator<T, A> &);
        friend bool operator<=(const Iterator<T, A> &, const Iterator<T, A> &);
        friend bool operator>=(const Iterator<T, A> &, const Iterator<T, A> &);

        Iterator<T, A> &operator+=(size_type);
        friend Iterator<T, A> operator+(const Iterator<T, A> &, size_type);
        friend Iterator<T, A> operator+(size_type, const Iterator<T, A> &);
        Iterator<T, A> &operator-=(size_type);
        friend Iterator<T, A> operator-(const Iterator<T, A> &, size_type);
        friend difference_type operator-(Iterator<T, A>, Iterator<T, A>);

        reference operator[](size_type) const;
    };
} // namespace storm::core
