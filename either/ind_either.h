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

template <typename Target, typename Mot, typename BaseLeft, typename BaseRight>
struct Height<IndEither_t<Target, Mot, BaseLeft, BaseRight>>
    : std::integral_constant<int,
                             std::max({height_v<Target>, height_v<Mot>, height_v<BaseLeft>, height_v<BaseRight>})> {};

template <typename Target1,
          typename Mot1,
          typename BaseLeft1,
          typename BaseRight1,
          typename Target2,
          typename Mot2,
          typename BaseLeft2,
          typename BaseRight2>
struct Equal<IndEither_t<Target1, Mot1, BaseLeft1, BaseRight1>, IndEither_t<Target2, Mot2, BaseLeft2, BaseRight2>>
    : std::bool_constant<equal_v<Target1, Target2> && equal_v<Mot1, Mot2> && equal_v<BaseLeft1, BaseLeft2> &&
                         equal_v<BaseRight1, BaseRight2>> {};

template <typename Target, typename Mot, typename BaseLeft, typename BaseRight>
struct Printer<IndEither_t<Target, Mot, BaseLeft, BaseRight>> {
    static void print(std::ostream& s) {
        s << "(ind-Either " << Print<Target>{} << ' ' << Print<Mot>{} << ' ' << Print<BaseLeft>{} << ' '
          << Print<BaseRight>{} << ')';
    }
};

template <typename Target, typename Mot, typename BaseLeft, typename BaseRight>
constexpr IndEither_t<Target, Mot, BaseLeft, BaseRight> ind_Either(const Pie<Target>& target,
                                                                   const Pie<Mot>& mot,
                                                                   const Pie<BaseLeft>& base_left,
                                                                   const Pie<BaseRight>& base_right) {
    return IndEither_t<Target, Mot, BaseLeft, BaseRight>{
        target.derived(), mot.derived(), base_left.derived(), base_right.derived()};
}

template <typename Derived, typename Mot, typename BaseLeft, typename BaseRight>
constexpr auto
Step_Ind_Either(const Right_t<Derived>& target, const Mot&, const BaseLeft&, const BaseRight& base_right) {
    return base_right(target.r_);
}

template <typename Derived, typename Mot, typename BaseLeft, typename BaseRight>
constexpr auto Step_Ind_Either(const Left_t<Derived>& target, const Mot&, const BaseLeft& base_left, const BaseRight&) {
    return base_left(target.l_);
}

template <typename Target, typename Mot, typename BaseLeft, typename BaseRight>
constexpr auto Step1(const IndEither_t<Target, Mot, BaseLeft, BaseRight>& ind_either) {
    assert(!is_neutral_v<Target>);
    return Step_Ind_Either(
        ComputeValue(ind_either.target_), ind_either.mot_, ind_either.base_left_, ind_either.base_right_);
}

template <typename Target, typename Mot, typename BaseLeft, typename BaseRight>
struct is_neutral<IndEither_t<Target, Mot, BaseLeft, BaseRight>> : std::bool_constant<is_neutral_v<Target>> {};

template <typename Target, typename Mot, typename BaseLeft, typename BaseRight>
constexpr auto synth1(const IndEither_t<Target, Mot, BaseLeft, BaseRight>& ind_either) {
    const auto target_type = ComputeValue(synth(ind_either.target_));
    assert(IsA(ind_either.mot_, Arrow(target_type, U)));
    assert(IsA(ind_either.base_left_,
               Pi(target_type.left_, [&ind_either](const auto& x) { return ind_either.mot_(left(x)); })));
    assert(IsA(ind_either.base_right_,
               Pi(target_type.right_, [&ind_either](const auto& y) { return ind_either.mot_(right(y)); })));
    return ind_either.mot_(ind_either.target_);
}
