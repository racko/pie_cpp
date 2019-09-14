#pragma once

#include "eq/eq.h"
#include "pie_base.h"
#include <ostream>
#include <type_traits>
#include <utility>

template <typename Derived>
struct Same_t : Pie<Same_t<Derived>> {
    using type = Derived;

    constexpr Same_t(Derived x) : x_{std::move(x)} {}

    Derived x_;
};

template <typename Derived1, typename Derived2>
constexpr bool equal(const Same_t<Derived1>& lhs, const Same_t<Derived2>& rhs, int& next_index) {
    return equal(lhs.x_, rhs.x_, next_index);
}

template <typename Derived>
void print(std::ostream& s, const Same_t<Derived>& same, int& next_index) {
    s << "(same " << InContext(same.x_, next_index) << ')';
}

template <typename Derived>
constexpr Same_t<Derived> same(const Pie<Derived>& x) {
    return Same_t<Derived>(x.derived());
}

template <typename DerivedT, typename DerivedFrom, typename DerivedTo, typename Derived>
constexpr bool IsA1(const Same_t<Derived>& value, const Eq_t<DerivedT, DerivedFrom, DerivedTo>& type, int& next_index) {
    return IsA1(value.x_, type.type_, next_index) && IsTheSameAs1(type.type_, type.from_, value.x_, next_index) &&
           IsTheSameAs1(type.type_, value.x_, type.to_, next_index);
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
