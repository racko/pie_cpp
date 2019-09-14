#pragma once

#include "pair/pair.h"
#include "pie_base.h"
#include <ostream>
#include <type_traits>
#include <utility>

template <typename Derived1, typename Derived2>
struct Cons_t : Pie<Cons_t<Derived1, Derived2>> {
    using car_type = Derived1;
    using cdr_type = Derived2;

    constexpr Cons_t(Derived1 car, Derived2 cdr) : car_{std::move(car)}, cdr_{std::move(cdr)} {}

    Derived1 car_;
    Derived2 cdr_;
};

template <typename Derived1, typename Derived2, typename Derived3, typename Derived4>
constexpr bool equal(const Cons_t<Derived1, Derived2>& lhs, const Cons_t<Derived3, Derived4>& rhs, int& next_index) {
    return equal(lhs.car_, rhs.car_, next_index) && equal(lhs.cdr_, rhs.cdr_, next_index);
}

template <typename Derived1, typename Derived2>
void print(std::ostream& s, const Cons_t<Derived1, Derived2>& cons, int& next_index) {
    s << "(cons " << InContext(cons.car_, next_index) << ' ' << InContext(cons.cdr_, next_index) << ')';
}

template <typename Derived1, typename Derived2>
constexpr Cons_t<Derived1, Derived2> cons(const Pie<Derived1>& car, const Pie<Derived2>& cdr) {
    return Cons_t<Derived1, Derived2>(car, cdr);
}

template <typename DerivedT1, typename DerivedT2, typename Derived1, typename Derived2>
constexpr bool
IsA1(const Cons_t<Derived1, Derived2>& value, const Pair_t<DerivedT1, DerivedT2>& type, int& next_index) {
    return IsA1(value.car_, type.car_t_, next_index) && IsA1(value.cdr_, type.cdr_t_, next_index);
}

template <typename Derived1, typename Derived2>
struct is_normal<Cons_t<Derived1, Derived2>> : std::bool_constant<is_normal_v<Derived1> && is_normal_v<Derived2>> {};

template <typename Derived1, typename Derived2>
struct is_value<Cons_t<Derived1, Derived2>> : std::true_type {};

template <typename Car, typename Cdr>
struct normalize_result1<Cons_t<Car, Cdr>, false> {
    using type = Cons_t<normalize_result_t<Car>, normalize_result_t<Cdr>>;
};

template <typename Car, typename Cdr>
constexpr normalize_result_t<Cons_t<Car, Cdr>> Normalize(const Cons_t<Car, Cdr> type, std::false_type /*is_normal*/) {
    return Cons_t(Normalize(type.car_), Normalize(type.cdr_));
}

template <typename CarType, typename CdrType>
struct synth_result<Cons_t<CarType, CdrType>> {
    using type = Pair_t<synth_result_t<CarType>, synth_result_t<CdrType>>;
};

template <typename CarType, typename CdrType>
constexpr synth_result_t<Cons_t<CarType, CdrType>> synth1(const Cons_t<CarType, CdrType>& x, int& next_index) {
    return Pair(synth1(x.car_, next_index), synth1(x.cdr_, next_index));
}
