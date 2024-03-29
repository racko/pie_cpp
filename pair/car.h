#pragma once

#include "pie_base.h"
#include <ostream>
#include <type_traits>
#include <utility>

template <typename Derived>
struct Car_t : Pie<Car_t<Derived>> {
    constexpr Car_t(Derived cons) : height_{cons.height_}, cons_{std::move(cons)} {}

    int height_;
    Derived cons_;
};

template <typename Derived1, typename Derived2>
constexpr bool equal(const Car_t<Derived1>& lhs, const Car_t<Derived2>& rhs) {
    return lhs.cons_ == rhs.cons_;
}

template <typename Derived>
void print(std::ostream& s, const Car_t<Derived>& car) {
    s << "(car " << car.cons_ << ')';
}

template <typename Derived>
constexpr Car_t<Derived> car(const Pie<Derived>& cons) {
    return Car_t<Derived>(cons.derived());
}

template <typename Derived>
struct step_result<Car_t<Derived>> {
    using type = typename compute_value_result_t<Derived>::car_type;
};

template <typename Derived>
constexpr step_result_t<Car_t<Derived>> Step1(const Car_t<Derived>& car) {
    assert(!is_neutral_v<Derived>);
    return ComputeValue(car.cons_).car_;
}

template <typename Derived>
struct is_neutral<Car_t<Derived>> : std::bool_constant<is_neutral_v<Derived>> {};

template <typename ConsType>
struct synth_result<Car_t<ConsType>, std::enable_if_t<can_synth_v<ConsType>>> {
    using type = typename compute_value_result_t<synth_result_t<ConsType>>::arg_type;
};

template <typename ConsType, typename = std::enable_if_t<can_synth_v<ConsType>>>
constexpr synth_result_t<Car_t<ConsType>> synth1(const Car_t<ConsType>& x) {
    return ComputeValue(synth(x.cons_)).arg_;
}
