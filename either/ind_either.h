#pragma once

#include "either/either.h"
#include "either/left.h"
#include "either/right.h"
#include "lambda/pi.h"
#include "pie_base.h"
#include <ostream>
#include <utility>

template <typename Target, typename Mot, typename BaseLeft, typename BaseRight>
struct IndEither_t : Pie<IndEither_t<Target, Mot, BaseLeft, BaseRight>> {
    constexpr IndEither_t(const Target& target, const Mot& mot, const BaseLeft& base_left, const BaseRight& base_right)
        : target_{target}, mot_{mot}, base_left_{base_left}, base_right_{base_right} {}

    Target target_;
    Mot mot_;
    BaseLeft base_left_;
    BaseRight base_right_;
};

template <typename Target1,
          typename Mot1,
          typename BaseLeft1,
          typename BaseRight1,
          typename Target2,
          typename Mot2,
          typename BaseLeft2,
          typename BaseRight2>
constexpr bool equal(const IndEither_t<Target1, Mot1, BaseLeft1, BaseRight1>& lhs,
                     const IndEither_t<Target2, Mot2, BaseLeft2, BaseRight2>& rhs,
                     int& next_index) {
    return equal(lhs.target_, rhs.target_, next_index) && equal(lhs.mot_, rhs.mot_, next_index) &&
           equal(lhs.base_left_, rhs.base_left_, next_index) && equal(lhs.base_right_, rhs.base_right_, next_index);
}

template <typename Target, typename Mot, typename BaseLeft, typename BaseRight>
void print(std::ostream& s, const IndEither_t<Target, Mot, BaseLeft, BaseRight>& x, int& next_index) {
    s << "(ind-Either " << InContext(x.target_, next_index) << ' ' << InContext(x.mot_, next_index) << ' '
      << InContext(x.base_left_, next_index) << ' ' << InContext(x.base_right_, next_index) << ')';
}

template <typename Target, typename Mot, typename BaseLeft, typename BaseRight>
constexpr IndEither_t<Target, Mot, BaseLeft, BaseRight> ind_Either(const Pie<Target>& target,
                                                                   const Pie<Mot>& mot,
                                                                   const Pie<BaseLeft>& base_left,
                                                                   const Pie<BaseRight>& base_right) {
    return IndEither_t<Target, Mot, BaseLeft, BaseRight>{
        target.derived(), mot.derived(), base_left.derived(), base_right.derived()};
}

template <typename Target, typename Mot, typename BaseLeft, typename BaseRight>
struct step_Ind_Either_result;

template <typename Target, typename Mot, typename BaseLeft, typename BaseRight>
using step_Ind_Either_result_t = typename step_Ind_Either_result<Target, Mot, BaseLeft, BaseRight>::type;

template <typename Derived, typename Mot, typename BaseLeft, typename BaseRight>
struct step_Ind_Either_result<Right_t<Derived>, Mot, BaseLeft, BaseRight> {
    using type = apply_result_t<BaseRight, Derived>;
};

template <typename Derived, typename Mot, typename BaseLeft, typename BaseRight>
constexpr step_Ind_Either_result_t<Right_t<Derived>, Mot, BaseLeft, BaseRight>
Step_Ind_Either(const Right_t<Derived>& target, const Mot&, const BaseLeft&, const BaseRight& base_right) {
    return base_right(target.r_);
}

template <typename Derived, typename Mot, typename BaseLeft, typename BaseRight>
struct step_Ind_Either_result<Left_t<Derived>, Mot, BaseLeft, BaseRight> {
    using type = apply_result_t<BaseLeft, Derived>;
};

template <typename Derived, typename Mot, typename BaseLeft, typename BaseRight>
constexpr step_Ind_Either_result_t<Left_t<Derived>, Mot, BaseLeft, BaseRight>
Step_Ind_Either(const Left_t<Derived>& target, const Mot&, const BaseLeft& base_left, const BaseRight&) {
    return base_left(target.l_);
}

template <typename Target, typename Mot, typename BaseLeft, typename BaseRight>
struct step_result<IndEither_t<Target, Mot, BaseLeft, BaseRight>> {
    using type = step_Ind_Either_result_t<compute_value_result_t<Target>, Mot, BaseLeft, BaseRight>;
};

template <typename Target, typename Mot, typename BaseLeft, typename BaseRight>
constexpr step_result_t<IndEither_t<Target, Mot, BaseLeft, BaseRight>>
Step(const IndEither_t<Target, Mot, BaseLeft, BaseRight>& ind_either) {
    assert(!is_neutral_v<Target>);
    return Step_Ind_Either(
        ComputeValue(ind_either.target_), ind_either.mot_, ind_either.base_left_, ind_either.base_right_);
}

template <typename Target, typename Mot, typename BaseLeft, typename BaseRight>
struct is_neutral<IndEither_t<Target, Mot, BaseLeft, BaseRight>> : std::bool_constant<is_neutral_v<Target>> {};

template <typename Target, typename Mot, typename BaseLeft, typename BaseRight>
struct synth_result<IndEither_t<Target, Mot, BaseLeft, BaseRight>> {
    using type = apply_result_t<Mot, Target>;
};

template <typename Target, typename Mot, typename BaseLeft, typename BaseRight>
constexpr synth_result_t<IndEither_t<Target, Mot, BaseLeft, BaseRight>>
synth1(const IndEither_t<Target, Mot, BaseLeft, BaseRight>& ind_either, int& next_index) {
    const auto target_type = ComputeValue(synth1(ind_either.target_, next_index));
    assert(IsA1(ind_either.mot_, Arrow(target_type, U), next_index));
    assert(IsA1(ind_either.base_left_,
                Pi(target_type.left_, [&ind_either](const auto& x) { return ind_either.mot_(left(x)); }),
                next_index));
    assert(IsA1(ind_either.base_right_,
                Pi(target_type.right_, [&ind_either](const auto& y) { return ind_either.mot_(right(y)); }),
                next_index));
    return ind_either.mot_(ind_either.target_);
}

