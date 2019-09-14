#pragma once

#include "nat/add1.h"
#include "nat/int32.h"
#include "nat/nat.h"
#include "nat/zero.h"
#include "pie_base.h"
#include <ostream>
#include <utility>

template <typename Target, typename Base, typename Step>
struct IterNat_t : Pie<IterNat_t<Target, Base, Step>> {
    constexpr IterNat_t(const Target& target, const Base& base, const Step& step)
        : target_{target}, base_{base}, step_{step} {}

    Target target_;
    Base base_;
    Step step_;
};

template <typename Target1, typename Base1, typename Step1, typename Target2, typename Base2, typename Step2>
constexpr bool
equal(const IterNat_t<Target1, Base1, Step1>& lhs, const IterNat_t<Target2, Base2, Step2>& rhs, int& next_index) {
    return equal(lhs.target_, rhs.target_, next_index) && equal(lhs.base_, rhs.base_, next_index) &&
           equal(lhs.step_, rhs.step_, next_index);
}

template <typename Target, typename Base, typename Step>
void print(std::ostream& s, const IterNat_t<Target, Base, Step>& x, int& next_index) {
    s << "(iter-Nat " << InContext(x.target_, next_index) << ' ' << InContext(x.base_, next_index) << ' '
      << InContext(x.step_, next_index) << ')';
}

template <typename Target, typename Base, typename Step>
constexpr IterNat_t<Target, Base, Step>
iter_Nat(const Pie<Target>& target, const Pie<Base>& base, const Pie<Step>& step) {
    return IterNat_t<Target, Base, Step>{target.derived(), base.derived(), step.derived()};
}

template <typename Target, typename Base, typename Step>
struct step_Iter_Nat_result;

template <typename Target, typename Base, typename Step>
using step_Iter_Nat_result_t = typename step_Iter_Nat_result<Target, Base, Step>::type;

template <typename Base, typename Step>
struct step_Iter_Nat_result<Int32<0>, Base, Step> {
    using type = Base;
};

template <typename Base, typename Step>
constexpr step_Iter_Nat_result_t<Int32<0>, Base, Step> Step_Iter_Nat(Int32<0>, const Base& base, const Step&) {
    return base;
}

template <std::int32_t N, typename Base, typename Step>
struct step_Iter_Nat_result<Int32<N>, Base, Step> {
    using type = std::invoke_result_t<Step, IterNat_t<Int32<N - 1>, Base, Step>>;
};

template <std::int32_t N, typename Base, typename Step>
constexpr step_Iter_Nat_result_t<Int32<N>, Base, Step> Step_Iter_Nat(Int32<N>, const Base& base, const Step& step) {
    return step(iter_Nat(Int32<N - 1>{}, base, step));
}

template <typename Base, typename Step>
struct step_Iter_Nat_result<Zero_t, Base, Step> {
    using type = Base;
};

template <typename Base, typename Step>
constexpr step_Iter_Nat_result_t<Zero_t, Base, Step> Step_Iter_Nat(Zero_t, const Base& base, const Step&) {
    return base;
}

template <typename Derived, typename Base, typename Step>
struct step_Iter_Nat_result<Add1_t<Derived>, Base, Step> {
    using type = std::invoke_result_t<Step, IterNat_t<Derived, Base, Step>>;
};

template <typename Derived, typename Base, typename Step>
constexpr step_Iter_Nat_result_t<Add1_t<Derived>, Base, Step>
Step_Iter_Nat(const Add1_t<Derived>& target, const Base& base, const Step& step) {
    return step(iter_Nat(target.smaller_, base, step));
}

template <typename Target, typename Base, typename Step>
struct step_result<IterNat_t<Target, Base, Step>> {
    using type = step_Iter_Nat_result_t<compute_value_result_t<Target>, Base, Step>;
};

template <typename Target, typename Base, typename StepT>
constexpr step_result_t<IterNat_t<Target, Base, StepT>> Step(const IterNat_t<Target, Base, StepT>& iter_nat) {
    assert(!is_neutral_v<Target>);
    return Step_Iter_Nat(ComputeValue(iter_nat.target_), iter_nat.base_, iter_nat.step_);
}

template <typename Target, typename Base, typename Step>
struct is_neutral<IterNat_t<Target, Base, Step>> : std::bool_constant<is_neutral_v<Target>> {};

template <typename Target, typename Base, typename Step>
struct synth_result<IterNat_t<Target, Base, Step>> {
    using type = synth_result_t<Base>;
};

template <typename Target, typename Base, typename Step>
constexpr synth_result_t<IterNat_t<Target, Base, Step>> synth1(const IterNat_t<Target, Base, Step>& x,
                                                               int& next_index) {
    const auto base_type = synth1(x.base_, next_index);
    assert(IsA1(x.target_, Nat, next_index) && IsA1(x.step_, Arrow(base_type, base_type), next_index));
    return base_type;
}
