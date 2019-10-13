#pragma once

#include "pie_base.h"
#include <ostream>

template <typename Derived1, typename Derived2>
struct The_t : Pie<The_t<Derived1, Derived2>> {
    constexpr The_t(const Pie<Derived1>& type, const Pie<Derived2>& value) : type_(type), value_(value) {}

    Derived1 type_;
    Derived2 value_;
};

template <typename Derived1, typename Derived2>
struct Height<The_t<Derived1, Derived2>>
    : std::integral_constant<int, std::max(height_v<Derived1>, height_v<Derived2>)> {};

template <typename Derived1, typename Derived2, typename Derived3, typename Derived4>
struct Equal<The_t<Derived1, Derived2>, The_t<Derived3, Derived4>> : std::bool_constant<equal_v<Derived2, Derived4>> {};

template <typename Derived1, typename Derived2>
struct Printer<The_t<Derived1, Derived2>> {
    static void print(std::ostream& s) { s << "(the " << Print<Derived1>{} << ' ' << Print<Derived2>{} << ')'; }
};

template <typename Derived1, typename Derived2>
constexpr The_t<Derived1, Derived2> the(const Pie<Derived1>& type, const Pie<Derived2>& value) {
    return The_t<Derived1, Derived2>{type, value};
}

template <typename Type, typename Expr>
constexpr Expr Step1(const The_t<Type, Expr>& x) {
    return x.value_;
}

template <typename Type, typename Expr>
constexpr Type synth1(const The_t<Type, Expr>& x) {
    assert(IsAType(x.type_));
    assert(IsA(x.value_, x.type_));
    return x.type_;
}
