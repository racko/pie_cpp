#pragma once

#include "pair/sigma.h"
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

template <typename Derived1, typename Derived2>
struct Height<Cons_t<Derived1, Derived2>>
    : std::integral_constant<int, std::max(height_v<Derived1>, height_v<Derived2>)> {};

template <typename Derived1, typename Derived2, typename Derived3, typename Derived4>
struct Equal<Cons_t<Derived1, Derived2>, Cons_t<Derived3, Derived4>>
    : std::bool_constant<equal_v<Derived1, Derived3> && equal_v<Derived2, Derived4>> {};

template <typename Derived1, typename Derived2>
struct Printer<Cons_t<Derived1, Derived2>> {
    void print(std::ostream& s) { s << "(cons " << Print<Derived1>{} << ' ' << Print<Derived2>{} << ')'; }
};

template <typename Derived1, typename Derived2>
constexpr Cons_t<Derived1, Derived2> cons(const Pie<Derived1>& car, const Pie<Derived2>& cdr) {
    return Cons_t<Derived1, Derived2>(car, cdr);
}

template <typename DerivedT1, typename DerivedT2, typename Derived1, typename Derived2>
constexpr bool IsA1(const Cons_t<Derived1, Derived2>& value, const Sigma_t<DerivedT1, DerivedT2>& type) {
    return IsA(value.car_, type.arg_) && IsA(value.cdr_, type.result_(value.car_));
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
