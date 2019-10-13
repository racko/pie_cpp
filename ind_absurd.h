#pragma once

#include "absurd.h"
#include "pie_base.h"
#include <ostream>
#include <type_traits>
#include <utility>

template <typename Target, typename Mot>
struct IndAbsurd_t : Pie<IndAbsurd_t<Target, Mot>> {
    constexpr IndAbsurd_t(Target target, Mot mot) : target_{std::move(target)}, mot_{std::move(mot)} {}

    Target target_;
    Mot mot_;
};

template <typename Target, typename Mot>
struct Height<IndAbsurd_t<Target, Mot>> : std::integral_constant<int, std::max(height_v<Target>, height_v<Mot>)> {};

template <typename Target1, typename Mot1, typename Target2, typename Mot2>
struct Equal<IndAbsurd_t<Target1, Mot1>, IndAbsurd_t<Target2, Mot2>>
    : std::bool_constant<equal_v<Target1, Target2> && equal_v<Mot1, Mot2>> {};

template <typename Target, typename Mot>
struct Printer<IndAbsurd_t<Target, Mot>> {
    static void print(std::ostream& s) { s << "(ind-Absurd " << Print<Target>{} << ' ' << Print<Mot>{} << ')'; }
};

template <typename Target, typename Mot>
constexpr IndAbsurd_t<Target, Mot> ind_Absurd(const Pie<Target>& target, const Pie<Mot>& mot) {
    return IndAbsurd_t<Target, Mot>(target.derived(), mot.derived());
}

template <typename Target, typename Mot>
struct is_neutral<IndAbsurd_t<Target, Mot>> : std::bool_constant<is_neutral_v<Target>> {};

template <typename Target, typename Mot>
constexpr Mot synth1(const IndAbsurd_t<Target, Mot>& ind_absurd) {
    const auto& t = ind_absurd.target_;
    const auto& m = ind_absurd.mot_;

    assert(IsA(t, Absurd));
    assert(IsAType(m));

    return m;
}
