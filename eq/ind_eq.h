#pragma once

#include "eq/eq.h"
#include "eq/same.h"
#include "pie_base.h"
#include <ostream>
#include <type_traits>
#include <utility>

template <typename Target, typename Mot, typename Base>
struct IndEq_t : Pie<IndEq_t<Target, Mot, Base>> {
    constexpr IndEq_t(Target target, Mot mot, Base base)
        : target_{std::move(target)}, mot_{std::move(mot)}, base_{std::move(base)} {}

    Target target_;
    Mot mot_;
    Base base_;
};

template <typename Target, typename Mot, typename Base>
struct Height<IndEq_t<Target, Mot, Base>>
    : std::integral_constant<int, std::max({height_v<Target>, height_v<Mot>, height_v<Base>})> {};

template <typename Target1, typename Mot1, typename Base1, typename Target2, typename Mot2, typename Base2>
struct Equal<IndEq_t<Target1, Mot1, Base1>, IndEq_t<Target2, Mot2, Base2>>
    : std::bool_constant<equal_v<Target1, Target2> && equal_v<Mot1, Mot2> && equal_v<Base1, Base2>> {};

template <typename Target, typename Mot, typename Base>
struct Printer<IndEq_t<Target, Mot, Base>> {
    static void print(std::ostream& s) {
        s << "(ind-= " << Print<Target>{} << ' ' << Print<Mot>{} << ' ' << Print<Base>{} << ')';
    }
};

template <typename Target, typename Mot, typename Base>
constexpr IndEq_t<Target, Mot, Base> ind_Eq(const Pie<Target>& target, const Pie<Mot>& mot, const Pie<Base>& base) {
    return IndEq_t<Target, Mot, Base>(target.derived(), mot.derived(), base.derived());
}

template <typename Target, typename Mot, typename Base>
constexpr Base Step1(const IndEq_t<Target, Mot, Base>& ind_eq) {
    assert(!is_neutral_v<Target>);
    return ind_eq.base_;
}

template <typename Target, typename Mot, typename Base>
struct is_neutral<IndEq_t<Target, Mot, Base>> : std::bool_constant<is_neutral_v<Target>> {};

template <typename Target, typename Mot, typename Base>
constexpr auto synth1(const IndEq_t<Target, Mot, Base>& ind_eq) {
    const auto& t = ind_eq.target_;
    const auto& m = ind_eq.mot_;
    const auto& b = ind_eq.base_;

    const auto target_type = ComputeValue(synth(t));
    const auto& X = target_type.type_;
    const auto& from = target_type.from_;
    const auto& to = target_type.to_;
    assert(IsA(m, Pi(X, [X, from](const auto& x) { return Arrow(Eq(X, from, x), U); })));
    assert(IsA(b, m(from, same(from))));

    return m(to, t);
}
