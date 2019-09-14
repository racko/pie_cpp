#pragma once

#include "nat/nat.h"
#include "pie_base.h"
#include <ostream>
#include <utility>

template <typename Derived>
struct Add1_t : Pie<Add1_t<Derived>> {
    constexpr Add1_t(const Derived& smaller) : smaller_{smaller} {}

    Derived smaller_;
};

template <typename Derived1, typename Derived2>
constexpr bool equal(const Add1_t<Derived1>& lhs, const Add1_t<Derived2>& rhs, int& next_index) {
    return equal(lhs.smaller_, rhs.smaller_, next_index);
}

template <typename Derived>
void print(std::ostream& s, const Add1_t<Derived>& n, int& next_index) {
    s << "(add1 " << InContext(n.smaller_, next_index) << ')';
}

template <typename Derived>
constexpr Add1_t<Derived> add1(const Pie<Derived>& n) {
    return Add1_t<Derived>{n};
}

template <typename Derived>
constexpr bool IsA1(const Add1_t<Derived>& n, Nat_t, int& next_index) {
    return IsA1(n.smaller_, Nat, next_index);
}

template <typename Derived>
struct is_normal<Add1_t<Derived>> : std::bool_constant<is_normal_v<Derived>> {};

template <typename Derived>
struct is_value<Add1_t<Derived>> : std::true_type {};

template <typename Derived>
struct normalize_result1<Add1_t<Derived>, false> {
    using type = Add1_t<normalize_result_t<Derived>>;
};

template <typename Derived>
constexpr normalize_result_t<Add1_t<Derived>> Normalize(const Add1_t<Derived>& x, std::false_type /*is_normal*/) {
    return add1(Normalize(x.smaller_));
}

template <typename Derived>
struct synth_result<Add1_t<Derived>> {
    using type = Nat_t;
};

template <typename Derived>
constexpr synth_result_t<Add1_t<Derived>> synth1(Add1_t<Derived>, int&) {
    return Nat;
}
