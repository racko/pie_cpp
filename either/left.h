#pragma once

#include "either/either.h"
#include "pie_base.h"
#include <ostream>
#include <utility>

template <typename Derived>
struct Left_t : Pie<Left_t<Derived>> {
    constexpr Left_t(const Derived& l) : height_{l.height_}, l_{l} {}

    int height_;
    Derived l_;
};

template <typename Derived1, typename Derived2>
constexpr bool equal(const Left_t<Derived1>& lhs, const Left_t<Derived2>& rhs) {
    return lhs.l_ == rhs.l_;
}

template <typename Derived>
void print(std::ostream& s, const Left_t<Derived>& x) {
    s << "(left " << x.l_ << ')';
}

template <typename Derived>
constexpr Left_t<Derived> left(const Pie<Derived>& x) {
    return Left_t<Derived>{x.derived()};
}

template <typename Derived, typename L, typename R>
constexpr bool IsA1(const Left_t<Derived>& x, const Either_t<L, R>& t) {
    return IsA(x.l_, t.left_);
}

template <typename Derived>
struct is_normal<Left_t<Derived>> : std::bool_constant<is_normal_v<Derived>> {};

template <typename Derived>
struct is_value<Left_t<Derived>> : std::true_type {};

template <typename Derived>
struct normalize_result1<Left_t<Derived>, false> {
    using type = Left_t<normalize_result_t<Derived>>;
};

template <typename Derived>
constexpr normalize_result_t<Left_t<Derived>> Normalize(const Left_t<Derived>& x, std::false_type /*is_normal*/) {
    return left(Normalize(x.l_));
}
