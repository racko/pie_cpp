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

template <typename Derived1, typename Derived2>
constexpr bool equal(const Cdr_t<Derived1>& lhs, const Cdr_t<Derived2>& rhs, int& next_index) {
    return equal(lhs.cons_, rhs.cons_, next_index);
}

template <typename Derived>
void print(std::ostream& s, const Cdr_t<Derived>& cdr, int& next_index) {
    s << "(cdr " << InContext(cdr.cons_, next_index) << ')';
}

template <typename Derived>
constexpr Cdr_t<Derived> cdr(const Pie<Derived>& cons) {
    return Cdr_t<Derived>(cons);
}

template <typename Derived>
struct step_result<Cdr_t<Derived>> {
    using type = typename compute_value_result_t<Derived>::cdr_type;
};

template <typename Derived>
constexpr step_result_t<Cdr_t<Derived>> Step(const Cdr_t<Derived>& cdr) {
    assert(!is_neutral_v<Derived>);
    return ComputeValue(cdr.cons_).cdr_;
}

template <typename Derived>
struct is_neutral<Cdr_t<Derived>> : std::bool_constant<is_neutral_v<Derived>> {};

template <typename ConsType>
struct synth_result<Cdr_t<ConsType>> {
    // using type = std::invoke_result_t<typename compute_value_result_t<synth_result_t<ConsType>>::result_type,
    //                                  typename compute_value_result_t<ConsType>::car_type>;
    using type =
        std::invoke_result_t<typename compute_value_result_t<synth_result_t<ConsType>>::result_type, Car_t<ConsType>>;
};

template <typename ConsType>
constexpr synth_result_t<Cdr_t<ConsType>> synth1(const Cdr_t<ConsType>& x, int& next_index) {
    const auto cons_type = ComputeValue(synth1(x.cons_, next_index));
    // return cons_type.result_(is_neutral_v<ConsType> ? car(x.cons_) : ComputeValue(x.cons_).car_);
    return cons_type.result_(car(x.cons_));
}
