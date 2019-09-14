#pragma once

#include "pie_base.h"
#include <ostream>
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

template <typename Derived>
struct Car_t : Pie<Car_t<Derived>> {
    constexpr Car_t(Derived cons) : cons_{cons} {}

    Derived cons_;
};

template <typename Derived1, typename Derived2>
constexpr bool equal(const Car_t<Derived1>& lhs, const Car_t<Derived2>& rhs, int& next_index) {
    return equal(lhs.cons_, rhs.cons_, next_index);
}

template <typename Derived>
void print(std::ostream& s, const Car_t<Derived>& car, int& next_index) {
    s << "(car " << InContext(car.cons_, next_index) << ')';
}

template <typename Derived>
constexpr Car_t<Derived> car(const Pie<Derived>& cons) {
    return Car_t<Derived>(cons.derived());
}

template <typename Derived>
struct Cdr_t : Pie<Cdr_t<Derived>> {
    constexpr Cdr_t(Derived cons) : cons_{cons} {}

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

template <typename DerivedT1, typename DerivedT2, typename Derived1, typename Derived2>
constexpr bool IsA1(const Cons_t<Derived1, Derived2>& value, const Pair_t<DerivedT1, DerivedT2>& type, int& next_index) {
    return IsA1(value.car_, type.car_t_, next_index) && IsA1(value.cdr_, type.cdr_t_, next_index);
}

template <typename Derived1, typename Derived2>
constexpr bool IsAType1(const Pair_t<Derived1, Derived2>& type, int& next_index) {
    return IsAType1(type.car_t_, next_index) && IsAType1(type.cdr_t_, next_index);
}

template <typename Derived1, typename Derived2>
struct is_normal<Pair_t<Derived1, Derived2>> : std::bool_constant<is_normal_v<Derived1> && is_normal_v<Derived2>> {};

template <typename Derived1, typename Derived2>
struct is_value<Pair_t<Derived1, Derived2>> : std::true_type {};

template <typename Derived1, typename Derived2>
struct is_normal<Cons_t<Derived1, Derived2>> : std::bool_constant<is_normal_v<Derived1> && is_normal_v<Derived2>> {};

template <typename Derived1, typename Derived2>
struct is_value<Cons_t<Derived1, Derived2>> : std::true_type {};

template <typename Derived>
struct step_result<Car_t<Derived>> {
    using type = typename compute_value_result_t<Derived>::car_type;
};

template <typename Derived>
struct step_result<Cdr_t<Derived>> {
    using type = typename compute_value_result_t<Derived>::cdr_type;
};

template <typename Derived>
constexpr step_result_t<Car_t<Derived>> Step(const Car_t<Derived>& car) {
    assert(!is_neutral_v<Derived>);
    return ComputeValue(car.cons_).car_;
}

template <typename Derived>
constexpr step_result_t<Cdr_t<Derived>> Step(const Cdr_t<Derived>& cdr) {
    assert(!is_neutral_v<Derived>);
    return ComputeValue(cdr.cons_).cdr_;
}

template <typename Derived>
struct is_neutral<Car_t<Derived>> : std::bool_constant<is_neutral_v<Derived>> {};

template <typename Derived>
struct is_neutral<Cdr_t<Derived>> : std::bool_constant<is_neutral_v<Derived>> {};

template <typename Car, typename Cdr>
struct normalize_result1<Pair_t<Car, Cdr>, false> {
    using type = Pair_t<normalize_result_t<Car>, normalize_result_t<Cdr>>;
};

template <typename Car, typename Cdr>
struct normalize_result1<Cons_t<Car, Cdr>, false> {
    using type = Cons_t<normalize_result_t<Car>, normalize_result_t<Cdr>>;
};

template <typename Car, typename Cdr>
constexpr normalize_result_t<Pair_t<Car, Cdr>> Normalize(const Pair_t<Car, Cdr> type, std::false_type /*is_normal*/) {
    return Pair(Normalize(type.car_t_), Normalize(type.cdr_t_));
}

template <typename Car, typename Cdr>
constexpr normalize_result_t<Cons_t<Car, Cdr>> Normalize(const Cons_t<Car, Cdr> type, std::false_type /*is_normal*/) {
    return Cons_t(Normalize(type.car_), Normalize(type.cdr_));
}

template <typename CarType, typename CdrType>
struct synth_result<Pair_t<CarType, CdrType>> {
    using type = U_t;
};

template <typename CarType, typename CdrType>
constexpr synth_result_t<Pair_t<CarType, CdrType>> synth1(Pair_t<CarType, CdrType>, int&) {
    return U;
}

template <typename CarType, typename CdrType>
struct synth_result<Cons_t<CarType, CdrType>> {
    using type = Pair_t<synth_result_t<CarType>, synth_result_t<CdrType>>;
};

template <typename CarType, typename CdrType>
constexpr synth_result_t<Cons_t<CarType, CdrType>> synth1(const Cons_t<CarType, CdrType>& x, int& next_index) {
    return Pair(synth1(x.car_, next_index), synth1(x.cdr_, next_index));
}

template <typename ConsType>
struct synth_result<Car_t<ConsType>> {
    using type = typename synth_result_t<ConsType>::car_type;
};

template <typename ConsType>
constexpr synth_result_t<Car_t<ConsType>> synth1(const Car_t<ConsType>& x, int& next_index) {
    return synth1(x.cons_, next_index).car_t_;
}

template <typename ConsType>
struct synth_result<Cdr_t<ConsType>> {
    using type = typename synth_result_t<ConsType>::cdr_type;
};

template <typename ConsType>
constexpr synth_result_t<Cdr_t<ConsType>> synth1(const Cdr_t<ConsType>& x, int& next_index) {
    return synth1(x.cons_, next_index).cdr_t_;
}
