#pragma once

#include "pie_base.h"
#include <ostream>

struct Var_t : Pie<Var_t> {
    constexpr Var_t(const int id) : id_{id} {}

    int height_{};
    int id_;
};

constexpr bool equal(const Var_t lhs, const Var_t rhs) { return lhs.id_ == rhs.id_; }

inline void print(std::ostream& s, const Var_t x) { s << 'x' << x.id_; }

constexpr Var_t var(const int id) { return Var_t{id}; }

template <>
struct is_neutral<Var_t> : std::true_type {};
