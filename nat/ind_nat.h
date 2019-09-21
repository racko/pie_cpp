#pragma once

#include "lambda/pi.h"
#include "nat/add1.h"
#include "nat/int32.h"
#include "nat/nat.h"
#include "nat/zero.h"
#include "pie_base.h"
#include <ostream>
#include <utility>

template <typename Target, typename Mot, typename Base, typename Step>
struct IndNat_t : Pie<IndNat_t<Target, Mot, Base, Step>> {
    constexpr IndNat_t(const Target& target, const Mot& mot, const Base& base, const Step& step)
        : target_{target}, mot_{mot}, base_{base}, step_{step} {}

    Target target_;
    Mot mot_;
    Base base_;
    Step step_;
};

template <typename Target1,
          typename Mot1,
          typename Base1,
          typename Step1,
          typename Target2,
          typename Mot2,
          typename Base2,
          typename Step2>
constexpr bool equal(const IndNat_t<Target1, Mot1, Base1, Step1>& lhs,
                     const IndNat_t<Target2, Mot2, Base2, Step2>& rhs,
                     int& next_index) {
    return equal(lhs.target_, rhs.target_, next_index) && equal(lhs.mot_, rhs.mot_, next_index) &&
           equal(lhs.base_, rhs.base_, next_index) && equal(lhs.step_, rhs.step_, next_index);
}

template <typename Target, typename Mot, typename Base, typename Step>
void print(std::ostream& s, const IndNat_t<Target, Mot, Base, Step>& x, int& next_index) {
    s << "(ind-Nat " << InContext(x.target_, next_index) << ' ' << InContext(x.mot_, next_index) << ' '
      << InContext(x.base_, next_index) << ' ' << InContext(x.step_, next_index) << ')';
}

template <typename Target, typename Mot, typename Base, typename Step>
constexpr IndNat_t<Target, Mot, Base, Step>
ind_Nat(const Pie<Target>& target, const Pie<Mot>& mot, const Pie<Base>& base, const Pie<Step>& step) {
    return IndNat_t<Target, Mot, Base, Step>{target.derived(), mot.derived(), base.derived(), step.derived()};
}

template <typename Target, typename Mot, typename Base, typename Step>
struct step_Ind_Nat_result;

template <typename Target, typename Mot, typename Base, typename Step>
using step_Ind_Nat_result_t = typename step_Ind_Nat_result<Target, Mot, Base, Step>::type;

template <typename Mot, typename Base, typename Step>
struct step_Ind_Nat_result<Int32<0>, Mot, Base, Step> {
    using type = Base;
};

template <typename Mot, typename Base, typename Step>
constexpr step_Ind_Nat_result_t<Int32<0>, Mot, Base, Step>
Step_Ind_Nat(Int32<0>, const Mot&, const Base& base, const Step&) {
    return base;
}

template <std::int32_t N, typename Mot, typename Base, typename Step>
struct step_Ind_Nat_result<Int32<N>, Mot, Base, Step> {
    using type = std::invoke_result_t<Step, Int32<N - 1>, IndNat_t<Int32<N - 1>, Mot, Base, Step>>;
};

template <std::int32_t N, typename Mot, typename Base, typename Step>
constexpr step_Ind_Nat_result_t<Int32<N>, Mot, Base, Step>
Step_Ind_Nat(Int32<N>, const Mot& mot, const Base& base, const Step& step) {
    return step(Int32<N - 1>{}, ind_Nat(Int32<N - 1>{}, mot, base, step));
}

template <typename Mot, typename Base, typename Step>
struct step_Ind_Nat_result<Zero_t, Mot, Base, Step> {
    using type = Base;
};

template <typename Mot, typename Base, typename Step>
constexpr step_Ind_Nat_result_t<Zero_t, Mot, Base, Step>
Step_Ind_Nat(Zero_t, const Mot&, const Base& base, const Step&) {
    return base;
}

template <typename Derived, typename Mot, typename Base, typename Step>
struct step_Ind_Nat_result<Add1_t<Derived>, Mot, Base, Step> {
    using type = std::invoke_result_t<Step, Derived, IndNat_t<Derived, Mot, Base, Step>>;
};

template <typename Derived, typename Mot, typename Base, typename Step>
constexpr step_Ind_Nat_result_t<Add1_t<Derived>, Mot, Base, Step>
Step_Ind_Nat(const Add1_t<Derived>& target, const Mot& mot, const Base& base, const Step& step) {
    return step(target.smaller_, ind_Nat(target.smaller_, mot, base, step));
}

template <typename Target, typename Mot, typename Base, typename Step>
struct step_result<IndNat_t<Target, Mot, Base, Step>> {
    using type = step_Ind_Nat_result_t<compute_value_result_t<Target>, Mot, Base, Step>;
};

template <typename Target, typename Mot, typename Base, typename StepT>
constexpr step_result_t<IndNat_t<Target, Mot, Base, StepT>> Step(const IndNat_t<Target, Mot, Base, StepT>& ind_nat) {
    assert(!is_neutral_v<Target>);
    return Step_Ind_Nat(ComputeValue(ind_nat.target_), ind_nat.mot_, ind_nat.base_, ind_nat.step_);
}

template <typename Target, typename Mot, typename Base, typename Step>
struct is_neutral<IndNat_t<Target, Mot, Base, Step>> : std::bool_constant<is_neutral_v<Target>> {};

template <typename Target, typename Mot, typename Base, typename Step>
struct synth_result<IndNat_t<Target, Mot, Base, Step>> {
    using type = apply_result_t<Mot, Target>;
};

template <typename Target, typename Mot, typename Base, typename Step>
constexpr synth_result_t<IndNat_t<Target, Mot, Base, Step>> synth1(const IndNat_t<Target, Mot, Base, Step>& x,
                                                                   int& next_index) {
    assert(IsA1(x.target_, Nat, next_index));
    assert(IsA1(x.mot_, Arrow(Nat, U), next_index));
    assert(IsA1(x.base_, ComputeValue(x.mot_(zero)), next_index));
    assert(IsA1(x.step_,
                Pi(Nat, [&x](const auto& smaller) { return Arrow(x.mot_(smaller), x.mot_(add1(smaller))); }),
                next_index));
    return x.mot_(x.target_);
}
