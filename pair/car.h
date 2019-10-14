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
    static void print(std::ostream& s) { s << "(car " << Print<Derived>{} << ')'; }
};

template <typename Derived>
constexpr Car_t<Derived> car(const Pie<Derived>& cons) {
    return Car_t<Derived>(cons.derived());
}

template <typename Derived>
struct is_neutral<Car_t<Derived>> : std::bool_constant<is_neutral_v<Derived>> {};

template <typename Derived>
struct StepResult<Car_t<Derived>> {
    static_assert(!is_neutral_v<Derived>);
    using type = typename compute_value_t<Derived>::car_type;
};

template <typename ConsType>
struct Synth<Car_t<ConsType>, std::enable_if_t<can_synth_v<ConsType>>> {
    using type = typename compute_value_t<synth_t<ConsType>>::arg_type;
};
