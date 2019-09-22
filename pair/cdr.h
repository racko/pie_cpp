#pragma once

#include "pair/car.h"
#include "pie_base.h"
#include <ostream>
#include <type_traits>
#include <utility>

template <typename Derived>
struct Cdr_t : Pie<Cdr_t<Derived>> {
    constexpr Cdr_t(Derived cons) : height_{cons.height_}, cons_{std::move(cons)} {}

    int height_;
    Derived cons_;
};

template <typename Derived1, typename Derived2>
constexpr bool equal(const Cdr_t<Derived1>& lhs, const Cdr_t<Derived2>& rhs) {
    return equal(lhs.cons_, rhs.cons_);
}

template <typename Derived>
void print(std::ostream& s, const Cdr_t<Derived>& cdr) {
    s << "(cdr " << cdr.cons_ << ')';
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
constexpr step_result_t<Cdr_t<Derived>> Step1(const Cdr_t<Derived>& cdr) {
    assert(!is_neutral_v<Derived>);
    return ComputeValue(cdr.cons_).cdr_;
}

template <typename Derived>
struct is_neutral<Cdr_t<Derived>> : std::bool_constant<is_neutral_v<Derived>> {};

template <typename ConsType>
struct synth_result<Cdr_t<ConsType>, std::enable_if_t<can_synth_v<ConsType>>> {
    using type =
        std::invoke_result_t<typename compute_value_result_t<synth_result_t<ConsType>>::result_type, Car_t<ConsType>>;
};

template <typename ConsType, typename = std::enable_if_t<can_synth_v<ConsType>>>
constexpr synth_result_t<Cdr_t<ConsType>> synth1(const Cdr_t<ConsType>& x) {
    const auto cons_type = ComputeValue(synth(x.cons_));
    return cons_type.result_(car(x.cons_));
}
