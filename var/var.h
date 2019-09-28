#pragma once

#include "pie_base.h"
#include <ostream>

template <int id>
struct Var_t : Pie<Var_t<id>> {};

template <int id>
struct Height<Var_t<id>> : std::integral_constant<int, 0> {};

template <int id1, int id2>
constexpr bool equal(Var_t<id1>, Var_t<id2>) {
    return id1 == id2;
}

template <int id>
inline void print(std::ostream& s, Var_t<id>) {
    s << 'x' << id;
}

template <int id>
constexpr Var_t<id> var() {
    return Var_t<id>{};
}

template <int id>
struct is_neutral<Var_t<id>> : std::true_type {};
