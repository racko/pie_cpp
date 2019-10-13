#pragma once

#include "pie_base.h"
#include <ostream>
#include <type_traits>

template <typename L, typename R>
struct Either_t : Pie<Either_t<L, R>> {
    constexpr Either_t(L left, R right) : left_{std::move(left)}, right_{std::move(right)} {}
    L left_;
    R right_;
};

template <typename L, typename R>
struct Height<Either_t<L, R>> : std::integral_constant<int, std::max(height_v<L>, height_v<R>)> {};

template <typename L, typename R>
constexpr Either_t<L, R> Either(const Pie<L>& l, const Pie<R>& r) {
    return Either_t<L, R>{l.derived(), r.derived()};
}

template <typename L1, typename R1, typename L2, typename R2>
constexpr bool equal(const Either_t<L1, R1>& lhs, const Either_t<L2, R2>& rhs) {
    return lhs.left_ == rhs.left_ && lhs.right_ == rhs.right_;
}

template <typename L1, typename R1, typename L2, typename R2>
struct Equal<Either_t<L1, R1>, Either_t<L2, R2>> : std::bool_constant<equal_v<L1, L2> && equal_v<R1, R2>> {};

template <typename L, typename R>
struct Printer<Either_t<L, R>> {
    static void print(std::ostream& s) { s << "(Either " << Print<L>{} << ' ' << Print<R>{} << ')'; }
};

template <typename L, typename R>
constexpr bool IsAType1(const Either_t<L, R>& x) {
    return IsAType(x.left_) && IsAType(x.right_);
}

template <typename L, typename R>
struct is_normal<Either_t<L, R>> : std::bool_constant<is_normal_v<L> && is_normal_v<R>> {};

template <typename L, typename R>
struct is_value<Either_t<L, R>> : std::true_type {};

template <typename L, typename R>
constexpr U_t synth1(const Either_t<L, R>& t) {
    assert(IsAType(t));
    return U;
}

template <typename L, typename R>
constexpr auto Normalize(const Either_t<L, R>& type, std::false_type /*is_normal*/) {
    return Either(Normalize(type.left_), Normalize(type.right_));
}
