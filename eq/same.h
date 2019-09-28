#pragma once

#include "eq/eq.h"
#include "pie_base.h"
#include <ostream>
#include <type_traits>
#include <utility>

template <typename Derived>
struct Same_t : Pie<Same_t<Derived>> {
    using type = Derived;

    constexpr Same_t(Derived x) : height_{x.height_}, x_{std::move(x)} {}

    int height_;
    Derived x_;
};

template <typename Derived1, typename Derived2>
constexpr bool equal(const Same_t<Derived1>& lhs, const Same_t<Derived2>& rhs) {
    return lhs.x_ == rhs.x_;
}

template <typename Derived>
void print(std::ostream& s, const Same_t<Derived>& same) {
    s << "(same " << same.x_ << ')';
}

template <typename Derived>
constexpr Same_t<Derived> same(const Pie<Derived>& x) {
    return Same_t<Derived>(x.derived());
}

template <typename DerivedT, typename DerivedFrom, typename DerivedTo, typename Derived>
constexpr bool IsA1(const Same_t<Derived>& value, const Eq_t<DerivedT, DerivedFrom, DerivedTo>& type) {
    return IsA(value.x_, type.type_) && IsTheSameAs(type.type_, type.from_, value.x_) &&
           IsTheSameAs(type.type_, value.x_, type.to_);
}

template <typename Derived>
struct is_normal<Same_t<Derived>> : std::bool_constant<is_normal_v<Derived>> {};

template <typename Derived>
struct is_value<Same_t<Derived>> : std::true_type {};

template <typename Derived>
struct normalize_result1<Same_t<Derived>, false> {
    using type = Same_t<normalize_result_t<Derived>>;
};

template <typename Derived>
constexpr normalize_result_t<Same_t<Derived>> Normalize(const Same_t<Derived> x, std::false_type /*is_normal*/) {
    return same(Normalize(x.x_));
}
