#pragma once

#include "pie_base.h"
#include <ostream>
#include <type_traits>
#include <utility>

template <typename Derived>
struct Car_t : Pie<Car_t<Derived>> {
    constexpr Car_t(Derived cons) : cons_{std::move(cons)} {}

    Derived cons_;
};

template <typename Derived>
struct Height<Car_t<Derived>> : std::integral_constant<int, height_v<Derived>> {};

template <typename Derived1, typename Derived2>
struct Equal<Car_t<Derived1>, Car_t<Derived2>> : std::bool_constant<equal_v<Derived1, Derived2>> {};

template <typename Derived>
struct Printer<Car_t<Derived>> {
    void print(std::ostream& s) { s << "(car " << Print<Derived>{} << ')'; }
};

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
