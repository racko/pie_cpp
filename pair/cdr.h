#pragma once

#include "pair/car.h"
#include "pie_base.h"
#include <ostream>
#include <type_traits>
#include <utility>

template <typename Derived>
struct Cdr_t : Pie<Cdr_t<Derived>> {
    constexpr Cdr_t(Derived cons) : cons_{std::move(cons)} {}

    Derived cons_;
};

template <typename Derived>
struct Height<Cdr_t<Derived>> : std::integral_constant<int, height_v<Derived>> {};

template <typename Derived1, typename Derived2>
struct Equal<Cdr_t<Derived1>, Cdr_t<Derived2>> : std::bool_constant<equal_v<Derived1, Derived2>> {};

template <typename Derived>
struct Printer<Cdr_t<Derived>> {
    static void print(std::ostream& s) { s << "(cdr " << Print<Derived>{} << ')'; }
};

template <typename Derived>
constexpr Cdr_t<Derived> cdr(const Pie<Derived>& cons) {
    return Cdr_t<Derived>(cons);
}

template <typename Derived>
constexpr auto Step1(const Cdr_t<Derived>& cdr) {
    assert(!is_neutral_v<Derived>);
    return ComputeValue(cdr.cons_).cdr_;
}

template <typename Derived>
struct is_neutral<Cdr_t<Derived>> : std::bool_constant<is_neutral_v<Derived>> {};

template <typename ConsType, typename = std::enable_if_t<can_synth_v<ConsType>>>
constexpr auto synth1(const Cdr_t<ConsType>& x) {
    const auto cons_type = ComputeValue(synth(x.cons_));
    return cons_type.result_(car(x.cons_));
}
