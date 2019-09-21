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

template <typename Target1, typename Mot1, typename Base1, typename Target2, typename Mot2, typename Base2>
constexpr bool
equal(const IndEq_t<Target1, Mot1, Base1>& lhs, const IndEq_t<Target2, Mot2, Base2>& rhs, int& next_index) {
    return equal(lhs.target_, rhs.target_, next_index) && equal(lhs.mot_, rhs.mot_, next_index) &&
           equal(lhs.base_, rhs.base_, next_index);
}

template <typename Target, typename Mot, typename Base>
void print(std::ostream& s, const IndEq_t<Target, Mot, Base>& ind_eq, int& next_index) {
    s << "(ind-= " << InContext(ind_eq.target_, next_index) << ' ' << InContext(ind_eq.mot_, next_index) << ' '
      << InContext(ind_eq.base_, next_index) << ')';
}

template <typename Target, typename Mot, typename Base>
constexpr IndEq_t<Target, Mot, Base> ind_Eq(const Pie<Target>& target, const Pie<Mot>& mot, const Pie<Base>& base) {
    return IndEq_t<Target, Mot, Base>(target.derived(), mot.derived(), base.derived());
}

template <typename Target, typename Mot, typename Base>
struct step_result<IndEq_t<Target, Mot, Base>> {
    using type = Base;
};

template <typename Target, typename Mot, typename Base>
constexpr step_result_t<IndEq_t<Target, Mot, Base>> Step(const IndEq_t<Target, Mot, Base>& ind_eq) {
    assert(!is_neutral_v<Target>);
    return ind_eq.base_;
}

template <typename Target, typename Mot, typename Base>
struct is_neutral<IndEq_t<Target, Mot, Base>> : std::bool_constant<is_neutral_v<Target>> {};

template <typename Target, typename Mot, typename Base>
struct synth_result<IndEq_t<Target, Mot, Base>> {
    using type = apply_result_t<Mot, typename compute_value_result_t<synth_result_t<Target>>::to_t, Target>;
};

template <typename Target, typename Mot, typename Base>
constexpr synth_result_t<IndEq_t<Target, Mot, Base>> synth1(const IndEq_t<Target, Mot, Base>& ind_eq, int& next_index) {
    LOG("synth(" << ind_eq << ") ...");
    const auto& t = ind_eq.target_;
    const auto& m = ind_eq.mot_;
    const auto& b = ind_eq.base_;

    const auto target_type = ComputeValue(synth1(t, next_index));
    const auto& X = target_type.type_;
    const auto& from = target_type.from_;
    const auto& to = target_type.to_;
    assert(IsA1(m, Pi(X, [X, from](const auto& x) { return Arrow(Eq(X, from, x), U); }), next_index));
    assert(IsA1(b, ComputeValue(m(from, same(from))), next_index));
    const auto& result = m(to, t);

    LOG("synth(" << ind_eq << "): " << result);

    return result;
}
