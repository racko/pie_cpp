#pragma once

#include "nat/add1.h"
#include "nat/int32.h"
#include "nat/zero.h"
#include <ostream>
#include <utility>

template <>
struct Equal<Int32<0>, Zero_t> : std::true_type {};

template <>
struct Equal<Zero_t, Int32<0>> : std::true_type {};

template <typename Derived, std::int32_t N>
struct Equal<Add1_t<Derived>, Int32<N>> : std::bool_constant<equal_v<Derived, Int32<N - 1>>> {};

template <std::int32_t N, typename Derived>
struct Equal<Int32<N>, Add1_t<Derived>> : std::bool_constant<equal_v<Derived, Int32<N - 1>>> {};
