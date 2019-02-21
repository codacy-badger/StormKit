#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <storm/core/CRTP.hpp>
#include <storm/core/private/NamedTypeImpl.hpp>
#include <type_traits>

namespace storm::core {
    template<typename T>
    struct Incrementable : CRTP<T, Incrementable> {
        T &operator+=(T const &other) {
            this->underlying().get() += other.get();
            return this->underlying();
        }
    };

    template<typename T>
    struct PreIncrementable : CRTP<T, PreIncrementable> {
        T &operator++() {
            ++this->underlying().get();
            return this->underlying();
        }
    };

    template<typename T>
    struct Addable : CRTP<T, Addable> {
        T operator+(T const &other) const { return T(this->underlying().get() + other.get()); }
    };

    template<typename T>
    struct Subtractable : CRTP<T, Subtractable> {
        T operator-(T const &other) const { return T(this->underlying().get() - other.get()); }
    };

    template<typename T>
    struct Multiplicable : CRTP<T, Multiplicable> {
        T operator*(T const &other) const { return T(this->underlying().get() * other.get()); }
    };

    template<typename T>
    struct ArithmeticSkills : Incrementable<T>,
                              PreIncrementable<T>,
                              Addable<T>,
                              Subtractable<T>,
                              Multiplicable<T> {};

    template<typename T>
    struct Comparable : CRTP<T, Comparable> {
        bool operator<(T const &other) const { return this->underlying().get() < other.get(); }
        bool operator>(T const &other) const { return other.get() < this->underlying().get(); }
        bool operator<=(T const &other) const { return !(other.get() < this->underlying().get()); }
        bool operator>=(T const &other) const { return !(*this < other); }
        bool operator==(T const &other) const {
            return !(*this < other) && !(other.get() < this->underlying().get());
        }
        bool operator!=(T const &other) const { return !(*this == other); }
    };

    template<typename InitializerType>
    struct FromInitializerList {
        template<typename T>
        struct templ : CRTP<T, templ> {
            templ(const std::initializer_list<InitializerType> &list) {
                this->underlying().get() = list;
            }
        };
    };

    template<typename Destination>
    struct ImplicitlyConvertibleTo {
        template<typename T>
        struct templ : CRTP<T, templ> {
            operator Destination() const { return this->underlying().get(); }
        };
    };

    template<typename T>
    struct Hashable {
        static constexpr bool is_hashable = true;
    };

    template<typename NamedType_>
    struct FunctionCallable;

    template<typename T, typename Parameter, template<typename> class... Skills>
    struct FunctionCallable<NamedType<T, Parameter, Skills...>>
          : CRTP<NamedType<T, Parameter, Skills...>, FunctionCallable> {
        operator T const &() const { return this->underlying().get(); }
        operator T &() { return this->underlying().get(); }
    };

    template<typename NamedType_>
    struct MethodCallable;

    template<typename T, typename Parameter, template<typename> class... Skills>
    struct MethodCallable<NamedType<T, Parameter, Skills...>>
          : CRTP<NamedType<T, Parameter, Skills...>, MethodCallable> {
        T const *operator->() const { return std::addressof(this->underlying().get()); }
        T *operator->() { return std::addressof(this->underlying().get()); }
    };

    template<typename NamedType_>
    struct Callable : FunctionCallable<NamedType_>, MethodCallable<NamedType_> {};

    template<typename T>
    using is_named_type = std::is_base_of<NamedTypeTrait, T>;

    template<typename T>
    inline constexpr bool is_named_type_v = is_named_type<T>::value;
} // namespace storm::core

namespace std {
    template<typename T, typename Parameter, template<typename> class... Skills>
    struct hash<storm::core::NamedType<T, Parameter, Skills...>> {
        using NamedType       = storm::core::NamedType<T, Parameter, Skills...>;
        using checkIfHashable = typename std::enable_if<NamedType::is_hashable, void>::type;

        size_t operator()(storm::core::NamedType<T, Parameter, Skills...> const &x) const {
            return std::hash<T>()(x.get());
        }
    };
} // namespace std
