#pragma once

#include "either/either.h"
#include "pie_base.h"
#include <ostream>
#include <utility>

template <typename Derived>
struct Right_t : Pie<Right_t<Derived>> {
    constexpr Right_t(const Derived& r) : height_{r.height_}, r_{r} {}

    int height_;
    Derived r_;
};

template <typename Derived1, typename Derived2>
constexpr bool equal(const Right_t<Derived1>& lhs, const Right_t<Derived2>& rhs) {
    return lhs.r_ == rhs.r_;
}

template <typename Derived>
void print(std::ostream& s, const Right_t<Derived>& x) {
    s << "(right " << x.r_ << ')';
}

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
struct normalize_result1<Right_t<Derived>, false> {
    using type = Right_t<normalize_result_t<Derived>>;
};

template <typename Derived>
constexpr normalize_result_t<Right_t<Derived>> Normalize(const Right_t<Derived>& x, std::false_type /*is_normal*/) {
    return right(Normalize(x.r_));
}
