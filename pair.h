#pragma once

#include "pie_base.h"
#include <ostream>
#include <utility>

template <typename Derived1, typename Derived2>
struct Pair_t : Pie<Pair_t<Derived1, Derived2>> {
    using car_type = Derived1;
    using cdr_type = Derived2;

    Pair_t(Derived1 car_t, Derived2 cdr_t) : car_t_{std::move(car_t)}, cdr_t_{std::move(cdr_t)} {}

    Derived1 car_t_;
    Derived2 cdr_t_;
};

template <typename Derived1, typename Derived2, typename Derived3, typename Derived4>
bool operator==(const Pair_t<Derived1, Derived2>& lhs, const Pair_t<Derived3, Derived4>& rhs) {
    return lhs.car_t_ == rhs.car_t_ && lhs.cdr_t_ == rhs.cdr_t_;
}

template <typename Derived1, typename Derived2>
std::wostream& operator<<(std::wostream& s, const Pair_t<Derived1, Derived2>& type) {
    return s << "(Pair " << type.car_t_ << ' ' << type.cdr_t_ << ')';
}

template <typename Derived1, typename Derived2>
Pair_t<Derived1, Derived2> Pair(const Pie<Derived1>& car_t, const Pie<Derived2>& cdr_t) {
    return Pair_t<Derived1, Derived2>(car_t, cdr_t);
}

template <typename Derived1, typename Derived2>
struct Cons_t : Pie<Cons_t<Derived1, Derived2>> {
    using car_type = Derived1;
    using cdr_type = Derived2;

    Cons_t(Derived1 car, Derived2 cdr) : car_{std::move(car)}, cdr_{std::move(cdr)} {}

    Derived1 car_;
    Derived2 cdr_;
};

template <typename Derived1, typename Derived2, typename Derived3, typename Derived4>
bool operator==(const Cons_t<Derived1, Derived2>& lhs, const Cons_t<Derived3, Derived4>& rhs) {
    return lhs.car_ == rhs.car_ && lhs.cdr_ == rhs.cdr_;
}

template <typename Derived1, typename Derived2>
std::wostream& operator<<(std::wostream& s, const Cons_t<Derived1, Derived2>& cons) {
    return s << "(cons " << cons.car_ << ' ' << cons.cdr_ << ')';
}

template <typename Derived1, typename Derived2>
Cons_t<Derived1, Derived2> cons(const Pie<Derived1>& car, const Pie<Derived2>& cdr) {
    return Cons_t<Derived1, Derived2>(car, cdr);
}

template <typename Derived>
struct Car_t : Pie<Car_t<Derived>> {
    Car_t(Derived cons) : cons_{cons} {}

    Derived cons_;
};

template <typename Derived1, typename Derived2>
bool operator==(const Car_t<Derived1>& lhs, const Car_t<Derived2>& rhs) {
    return lhs.cons_ == rhs.cons_;
}

template <typename Derived>
std::wostream& operator<<(std::wostream& s, const Car_t<Derived>& car) {
    return s << "(car " << car.cons_ << ')';
}

template <typename Derived>
Car_t<Derived> car(const Pie<Derived>& cons) {
    return Car_t<Derived>(cons.derived());
}

template <typename Derived>
struct Cdr_t : Pie<Cdr_t<Derived>> {
    Cdr_t(Derived cons) : cons_{cons} {}

    Derived cons_;
};

template <typename Derived1, typename Derived2>
bool operator==(const Cdr_t<Derived1>& lhs, const Cdr_t<Derived2>& rhs) {
    return lhs.cons_ == rhs.cons_;
}

template <typename Derived>
std::wostream& operator<<(std::wostream& s, const Cdr_t<Derived>& cdr) {
    return s << "(cdr " << cdr.cons_ << ')';
}

template <typename Derived>
Cdr_t<Derived> cdr(const Pie<Derived>& cons) {
    return Cdr_t<Derived>(cons);
}

template <typename DerivedT1, typename DerivedT2, typename Derived1, typename Derived2>
bool IsA(const Cons_t<Derived1, Derived2>& value, const Pair_t<DerivedT1, DerivedT2>& type) {
    return IsA(value.car_, type.car_t_) && IsA(value.cdr_, type.cdr_t_);
}

template <typename Derived1, typename Derived2>
bool IsAType(const Pair_t<Derived1, Derived2>& type) {
    return IsAType(type.car_t_) && IsAType(type.cdr_t_);
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
step_result_t<Car_t<Derived>> Step(const Car_t<Derived>& car) {
    assert(!is_neutral_v<Derived>);
    return ComputeValue(car.cons_).car_;
}

template <typename Derived>
step_result_t<Cdr_t<Derived>> Step(const Cdr_t<Derived>& cdr) {
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
normalize_result_t<Pair_t<Car, Cdr>> Normalize(const Pair_t<Car, Cdr> type, std::false_type /*is_normal*/) {
    return Pair(Normalize(type.car_t_), Normalize(type.cdr_t_));
}

template <typename Car, typename Cdr>
normalize_result_t<Cons_t<Car, Cdr>> Normalize(const Cons_t<Car, Cdr> type, std::false_type /*is_normal*/) {
    return Cons_t(Normalize(type.car_), Normalize(type.cdr_));
}

template <typename CarType, typename CdrType>
struct synth_result<Pair_t<CarType, CdrType>> {
    using type = U_t;
};

template <typename CarType, typename CdrType>
synth_result_t<Pair_t<CarType, CdrType>> synth(Pair_t<CarType, CdrType>) {
    return U;
}

template <typename CarType, typename CdrType>
struct synth_result<Cons_t<CarType, CdrType>> {
    using type = Pair_t<synth_result_t<CarType>, synth_result_t<CdrType>>;
};

template <typename CarType, typename CdrType>
synth_result_t<Cons_t<CarType, CdrType>> synth(const Cons_t<CarType, CdrType>& x) {
    return Pair(synth(x.car_), synth(x.cdr_));
}

template <typename ConsType>
struct synth_result<Car_t<ConsType>> {
    using type = typename synth_result_t<ConsType>::car_type;
};

template <typename ConsType>
synth_result_t<Car_t<ConsType>> synth(const Car_t<ConsType>& x) {
    return synth(x.cons_).car_t_;
}

template <typename ConsType>
struct synth_result<Cdr_t<ConsType>> {
    using type = typename synth_result_t<ConsType>::cdr_type;
};

template <typename ConsType>
synth_result_t<Cdr_t<ConsType>> synth(const Cdr_t<ConsType>& x) {
    return synth(x.cons_).cdr_t_;
}
