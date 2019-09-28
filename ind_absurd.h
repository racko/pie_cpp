#pragma once

#include "absurd.h"
#include "pie_base.h"
#include <ostream>
#include <type_traits>
#include <utility>

template <typename Target, typename Mot>
struct IndAbsurd_t : Pie<IndAbsurd_t<Target, Mot>> {
    constexpr IndAbsurd_t(Target target, Mot mot)
        : height_{std::max(target.height_, mot.height_)}, target_{std::move(target)}, mot_{std::move(mot)} {}

    int height_;
    Target target_;
    Mot mot_;
};

template <typename Target1, typename Mot1, typename Target2, typename Mot2>
constexpr bool equal(const IndAbsurd_t<Target1, Mot1>& lhs, const IndAbsurd_t<Target2, Mot2>& rhs) {
    return lhs.target_ == rhs.target_ && lhs.mot_ == rhs.mot_;
}

template <typename Target, typename Mot>
void print(std::ostream& s, const IndAbsurd_t<Target, Mot>& ind_absurd) {
    s << "(ind-= " << ind_absurd.target_ << ' ' << ind_absurd.mot_ << ')';
}

template <typename Target, typename Mot>
constexpr IndAbsurd_t<Target, Mot> ind_Absurd(const Pie<Target>& target, const Pie<Mot>& mot) {
    return IndAbsurd_t<Target, Mot>(target.derived(), mot.derived());
}

template <typename Target, typename Mot>
struct is_neutral<IndAbsurd_t<Target, Mot>> : std::bool_constant<is_neutral_v<Target>> {};

template <typename Target, typename Mot>
struct synth_result<IndAbsurd_t<Target, Mot>> {
    using type = Mot;
};

template <typename Target, typename Mot>
constexpr synth_result_t<IndAbsurd_t<Target, Mot>> synth1(const IndAbsurd_t<Target, Mot>& ind_absurd) {
    const auto& t = ind_absurd.target_;
    const auto& m = ind_absurd.mot_;

    assert(IsA(t, Absurd));
    assert(IsAType(m));

    return m;
}
