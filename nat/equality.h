#pragma once

#include "nat/add1.h"
#include "nat/int32.h"
#include "nat/zero.h"
#include <ostream>
#include <utility>

constexpr bool equal(Int32<0>, Zero_t) { return true; }

constexpr bool equal(Zero_t, Int32<0>) { return true; }

template <typename Derived, std::int32_t N>
constexpr bool equal(const Add1_t<Derived>& lhs, Int32<N>) {
    return lhs.smaller_ == Int32<N - 1>{};
}

template <std::int32_t N, typename Derived>
constexpr bool equal(Int32<N> lhs, const Add1_t<Derived>& rhs) {
    return rhs == lhs;
}
