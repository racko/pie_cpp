#pragma once

#include "nat/nat.h"
#include "pie_base.h"
#include <cstdint>
#include <ostream>
#include <type_traits>

template <std::int32_t N>
struct Int32 : Pie<Int32<N>> {
    Int32() = default;
    int height_{};
};

template <std::int32_t N1, std::int32_t N2>
constexpr bool equal(Int32<N1>, Int32<N2>) {
    return N1 == N2;
}

template <std::int32_t N>
void print(std::ostream& s, Int32<N>) {
    s << N;
}

template <std::int32_t N>
constexpr Int32<N> nat() {
    return Int32<N>{};
}

template <std::int32_t N>
constexpr bool IsA1(Int32<N>, Nat_t) {
    return N >= 0;
}

template <std::int32_t N>
struct is_normal<Int32<N>> : std::true_type {};

template <std::int32_t N>
struct is_value<Int32<N>> : std::true_type {};

template <std::int32_t N>
struct synth_result<Int32<N>> {
    using type = Nat_t;
};

template <std::int32_t N>
constexpr synth_result_t<Int32<N>> synth1(const Int32<N> n) {
    assert(IsA(n, Nat));
    return Nat;
}
