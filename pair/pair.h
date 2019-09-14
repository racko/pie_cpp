#pragma once

#include "pie_base.h"
#include <ostream>
#include <type_traits>
#include <utility>

template <typename Derived1, typename Derived2>
struct Pair_t : Pie<Pair_t<Derived1, Derived2>> {
    using car_type = Derived1;
    using cdr_type = Derived2;

    constexpr Pair_t(Derived1 car_t, Derived2 cdr_t) : car_t_{std::move(car_t)}, cdr_t_{std::move(cdr_t)} {}

    Derived1 car_t_;
    Derived2 cdr_t_;
};

template <typename Derived1, typename Derived2, typename Derived3, typename Derived4>
constexpr bool equal(const Pair_t<Derived1, Derived2>& lhs, const Pair_t<Derived3, Derived4>& rhs, int& next_index) {
    return equal(lhs.car_t_, rhs.car_t_, next_index) && equal(lhs.cdr_t_, rhs.cdr_t_, next_index);
}

template <typename Derived1, typename Derived2>
void print(std::ostream& s, const Pair_t<Derived1, Derived2>& type, int& next_index) {
    s << "(Pair " << InContext(type.car_t_, next_index) << ' ' << InContext(type.cdr_t_, next_index) << ')';
}

template <typename Derived1, typename Derived2>
constexpr Pair_t<Derived1, Derived2> Pair(const Pie<Derived1>& car_t, const Pie<Derived2>& cdr_t) {
    return Pair_t<Derived1, Derived2>(car_t, cdr_t);
}

template <typename Derived1, typename Derived2>
constexpr bool IsAType1(const Pair_t<Derived1, Derived2>& type, int& next_index) {
    return IsAType1(type.car_t_, next_index) && IsAType1(type.cdr_t_, next_index);
}

template <typename Derived1, typename Derived2>
struct is_normal<Pair_t<Derived1, Derived2>> : std::bool_constant<is_normal_v<Derived1> && is_normal_v<Derived2>> {};

template <typename Derived1, typename Derived2>
struct is_value<Pair_t<Derived1, Derived2>> : std::true_type {};

template <typename Car, typename Cdr>
struct normalize_result1<Pair_t<Car, Cdr>, false> {
    using type = Pair_t<normalize_result_t<Car>, normalize_result_t<Cdr>>;
};

template <typename Car, typename Cdr>
constexpr normalize_result_t<Pair_t<Car, Cdr>> Normalize(const Pair_t<Car, Cdr> type, std::false_type /*is_normal*/) {
    return Pair(Normalize(type.car_t_), Normalize(type.cdr_t_));
}

template <typename CarType, typename CdrType>
struct synth_result<Pair_t<CarType, CdrType>> {
    using type = U_t;
};

template <typename CarType, typename CdrType>
constexpr synth_result_t<Pair_t<CarType, CdrType>> synth1(Pair_t<CarType, CdrType>, int&) {
    return U;
}
