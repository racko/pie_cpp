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

template <typename Derived>
struct Height<Add1_t<Derived>> : std::integral_constant<int, height_v<Derived>> {};

template <typename Derived1, typename Derived2>
struct Equal<Add1_t<Derived1>, Add1_t<Derived2>> : std::bool_constant<equal_v<Derived1, Derived2>> {};

template <typename Derived>
struct Printer<Add1_t<Derived>> {
    static void print(std::ostream& s) { s << "(add1 " << Print<Derived>{} << ')'; }
};

template <typename Derived>
constexpr Add1_t<Derived> add1(const Pie<Derived>& n) {
    return Add1_t<Derived>{n};
}

template <typename Derived>
constexpr bool IsA1(const Add1_t<Derived>& n, Nat_t) {
    return IsA(n.smaller_, Nat);
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
constexpr synth_result_t<Add1_t<Derived>> synth1(const Add1_t<Derived>& n) {
    assert(IsA(n, Nat));
    return Nat;
}
