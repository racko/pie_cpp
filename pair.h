#pragma once

#include "pie_base.h"
#include <ostream>
#include <utility>

template <typename Derived1, typename Derived2>
struct Pair_t : Pie<Pair_t<Derived1, Derived2>> {
    Pair_t(Derived1 car_t, Derived2 cdr_t) : car_t_{std::move(car_t)}, cdr_t_{std::move(cdr_t)} {}

    Derived1 car_t_;
    Derived2 cdr_t_;
};

template <typename Derived1, typename Derived2, typename Derived3, typename Derived4>
bool operator==(const Pair_t<Derived1, Derived2>&, const Pair_t<Derived3, Derived4>&) {
    return false;
}

template <typename Derived1, typename Derived2>
bool operator==(const Pair_t<Derived1, Derived2>& lhs, const Pair_t<Derived1, Derived2>& rhs) {
    return lhs.car_t_ == rhs.car_t_ && lhs.cdr_t_ == rhs.cdr_t_;
}

template <typename Derived1, typename Derived2>
std::ostream& operator<<(std::ostream& s, const Pair_t<Derived1, Derived2>& type) {
    return s << "(Pair " << type.car_t_ << ' ' << type.cdr_t_ << ')';
}

template <typename Derived1, typename Derived2>
Pair_t<Derived1, Derived2> Pair(const Pie<Derived1>& car_t, const Pie<Derived2>& cdr_t) {
    return Pair_t<Derived1, Derived2>(car_t, cdr_t);
}

template <typename Derived1, typename Derived2>
struct Cons_t : Pie<Cons_t<Derived1, Derived2>> {
    Cons_t(Derived1 car, Derived2 cdr) : car_{std::move(car)}, cdr_{std::move(cdr)} {}

    Derived1 car_;
    Derived2 cdr_;
};

template <typename Derived1, typename Derived2, typename Derived3, typename Derived4>
bool operator==(const Cons_t<Derived1, Derived2>&, const Cons_t<Derived3, Derived4>&) {
    return false;
}

template <typename Derived1, typename Derived2>
bool operator==(const Cons_t<Derived1, Derived2>& lhs, const Cons_t<Derived1, Derived2>& rhs) {
    return lhs.car_ == rhs.car_ && lhs.cdr_ == rhs.cdr_;
}

template <typename Derived1, typename Derived2>
std::ostream& operator<<(std::ostream& s, const Cons_t<Derived1, Derived2>& cons) {
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
bool operator==(const Car_t<Derived1>&, const Car_t<Derived2>&) {
    return false;
}

template <typename Derived>
bool operator==(const Car_t<Derived>& lhs, const Car_t<Derived>& rhs) {
    return lhs.cons_ == rhs.cons_;
}

template <typename Derived>
std::ostream& operator<<(std::ostream& s, const Car_t<Derived>& car) {
    return s << "(car " << car.cons_ << ')';
}

template <typename Derived>
Car_t<Derived> car(const Pie<Derived>& cons) {
    return Car_t<Derived>(cons);
}

template <typename Derived>
struct Cdr_t : Pie<Cdr_t<Derived>> {
    Cdr_t(Derived cons) : cons_{cons} {}

    Derived cons_;
};

template <typename Derived1, typename Derived2>
bool operator==(const Cdr_t<Derived1>&, const Cdr_t<Derived2>&) {
    return false;
}

template <typename Derived>
bool operator==(const Cdr_t<Derived>& lhs, const Cdr_t<Derived>& rhs) {
    return lhs.cons_ == rhs.cons_;
}

template <typename Derived>
std::ostream& operator<<(std::ostream& s, const Cdr_t<Derived>& cdr) {
    return s << "(cdr " << cdr.cons_ << ')';
}

template <typename Derived>
Cdr_t<Derived> cdr(const Pie<Derived>& cons) {
    return Cdr_t<Derived>(cons);
}

template <typename DerivedT1, typename DerivedT2, typename Derived1, typename Derived2>
bool IsA_For_Values(const Cons_t<Derived1, Derived2>& value, const Pair_t<DerivedT1, DerivedT2>& type) {
    return IsA(value.car_, type.car_t_) && IsA(value.cdr_, type.cdr_t_);
}

template <typename DerivedT1,
          typename DerivedT2,
          typename Derived1,
          typename Derived2,
          typename Derived3,
          typename Derived4>
bool IsTheSameAs_For_Values(const Pair_t<DerivedT1, DerivedT2>& type,
                            const Cons_t<Derived1, Derived2>& lhs,
                            const Cons_t<Derived3, Derived4>& rhs) {
    return IsTheSameAs(type.car_t_, lhs.car_, rhs.car_) && IsTheSameAs(type.cdr_t_, lhs.cdr_, rhs.cdr_);
}

template <typename Derived1, typename Derived2>
bool IsAType_For_Values(const Pair_t<Derived1, Derived2>& type) {
    return IsAType(type.car_t_) && IsAType(type.cdr_t_);
}

template <typename Derived1, typename Derived2, typename Derived3, typename Derived4>
bool AreTheSameType_For_Values(const Pair_t<Derived1, Derived2>& type1, const Pair_t<Derived3, Derived4>& type2) {
    return AreTheSameType(type1.car_t_, type2.car_t_) && AreTheSameType(type1.cdr_t_, type2.cdr_t_);
}

template <typename DerivedT1, typename DerivedT2, typename Derived1, typename Derived2>
bool IsANormal(const Cons_t<Derived1, Derived2>& value, const Pair_t<DerivedT1, DerivedT2>& type) {
    return IsANormal(value.car_, type.car_t_) && IsANormal(value.cdr_, type.cdr_t_);
}

template <typename Derived1, typename Derived2>
bool IsANormalType(const Pair_t<Derived1, Derived2>& type) {
    return IsANormal(type.car_t_) && IsANormalType(type.cdr_t_);
}

template <typename Derived1, typename Derived2, typename Derived3, typename Derived4>
bool IsNormalFormOfType(const Pair_t<Derived1, Derived2>& type1, const Pair_t<Derived3, Derived4>& type2) {
    return IsNormalFormOfType(type1.car_t_, type2.car_t_) && IsNormalFormOfType(type1.cdr_t_, type2.cdr_t_);
}

template <typename DerivedT1, typename DerivedT2, typename Derived1, typename Derived2>
bool IsAValue(const Pair_t<DerivedT1, DerivedT2>& type, const Cons_t<Derived1, Derived2>& value) {
    return IsA(value.car_, type.car_t_) && IsA(value.cdr_, type.cdr_t_);
}

template <typename Derived1, typename Derived2>
Pair_t<Derived1, Derived2> ComputeValue(const Pair_t<Derived1, Derived2>& type) {
    return type;
}

template <typename Derived1, typename Derived2>
Cons_t<Derived1, Derived2> ComputeValue(const Cons_t<Derived1, Derived2>& value) {
    return value;
}

template <typename Derived>
auto ComputeValue(const Car_t<Derived>& car) {
    return ComputeValue(ComputeValue(car.cons_).car_);
}

template <typename Derived>
auto ComputeValue(const Cdr_t<Derived>& cdr) {
    return ComputeValue(ComputeValue(cdr.cons_).cdr_);
}

template <typename Derived1, typename Derived2>
Pair_t<Derived1, Derived2> Normalize(const Pair_t<Derived1, Derived2>& type) {
    return type;
}

template <typename Derived1, typename Derived2>
Cons_t<Derived1, Derived2> Normalize(const Cons_t<Derived1, Derived2>& value) {
    return value;
}

template <typename Derived>
auto Normalize(const Car_t<Derived>& car) {
    return Normalize(ComputeValue(car.cons_).car_);
}

template <typename Derived>
auto Normalize(const Cdr_t<Derived>& cdr) {
    return Normalize(ComputeValue(cdr.cons_).cdr_);
}
