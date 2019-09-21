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
constexpr Either_t<L, R> Either(const Pie<L>& l, const Pie<R>& r) {
    return Either_t<L, R>{l.derived(), r.derived()};
}

template <typename L1, typename R1, typename L2, typename R2>
constexpr bool equal(const Either_t<L1, R1>& lhs, const Either_t<L2, R2>& rhs, int& next_index) {
    return equal(lhs.left_, rhs.left_, next_index) && equal(lhs.right_, rhs.right_, next_index);
}

template <typename L, typename R>
inline void print(std::ostream& s, const Either_t<L, R>& x, int& next_index) {
    s << "Either(" << InContext(x.left_, next_index) << ' ' << InContext(x.right_, next_index) << ')';
}

template <typename L, typename R>
constexpr bool IsAType1(const Either_t<L, R>& x, int& next_index) {
    return IsAType1(x.left_, next_index) && IsAType1(x.right_, next_index);
}

template <typename L, typename R>
struct is_normal<Either_t<L, R>> : std::bool_constant<is_normal_v<L> && is_normal_v<R>> {};

template <typename L, typename R>
struct is_value<Either_t<L, R>> : std::true_type {};

template <typename L, typename R>
struct synth_result<Either_t<L, R>> {
    using type = U_t;
};

template <typename L, typename R>
constexpr synth_result_t<Either_t<L, R>> synth1(Either_t<L, R>, int&) {
    return U;
}

template <typename L, typename R>
struct normalize_result1<Either_t<L, R>, false> {
    using type = Either_t<normalize_result_t<L>, normalize_result_t<R>>;
};

template <typename L, typename R>
constexpr normalize_result_t<Either_t<L, R>> Normalize(const Either_t<L, R>& type,
                                                             std::false_type /*is_normal*/) {
    return Either(Normalize(type.left_), Normalize(type.right_));
}