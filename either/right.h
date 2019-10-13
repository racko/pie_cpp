#pragma once

#include "either/either.h"
#include "pie_base.h"
#include <ostream>
#include <utility>

template <typename Derived>
struct Right_t : Pie<Right_t<Derived>> {
    constexpr Right_t(const Derived& r) : r_{r} {}

    Derived r_;
};

template <typename Derived>
struct Height<Right_t<Derived>> : std::integral_constant<int, height_v<Derived>> {};

template <typename Derived1, typename Derived2>
struct Equal<Right_t<Derived1>, Right_t<Derived2>> : std::bool_constant<equal_v<Derived1, Derived2>> {};

template <typename Derived>
static void print(std::ostream& s, const Right_t<Derived>& x) {
    s << "(right " << x.r_ << ')';
}

template <typename Derived>
struct Printer<Right_t<Derived>> {
    static void print(std::ostream& s) { s << "(right " << Print<Derived>{} << ')'; }
};

template <typename Derived>
constexpr Right_t<Derived> right(const Pie<Derived>& x) {
    return Right_t<Derived>{x.derived()};
}

template <typename Derived, typename L, typename R>
constexpr bool IsA1(const Right_t<Derived>& x, const Either_t<L, R>& t) {
    return IsA(x.r_, t.right_);
}

template <typename Derived>
struct is_normal<Right_t<Derived>> : std::bool_constant<is_normal_v<Derived>> {};

template <typename Derived>
struct is_value<Right_t<Derived>> : std::true_type {};

template <typename Derived>
constexpr auto Normalize(const Right_t<Derived>& x, std::false_type /*is_normal*/) {
    return right(Normalize(x.r_));
}
