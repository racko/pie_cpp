#pragma once

#include "pie_base.h"
#include <ostream>

template<typename Derived1, typename Derived2>
struct The_t : Pie<The_t<Derived1, Derived2>> {
    The_t(const Pie<Derived1>& type, const Pie<Derived2>& value) : type_(type), value_(value)  {}

    Derived1 type_;
    Derived2 value_;
};

template<typename Derived1, typename Derived2>
std::ostream& operator<<(std::ostream& s, const The_t<Derived1, Derived2>& x) { return s << "(the " << x.type_ << ' ' << x.value_ << ')'; }

template<typename Derived1, typename Derived2>
The_t<Derived1, Derived2> the(const Pie<Derived1>& type, const Pie<Derived2>& value) {
    return The_t<Derived1, Derived2>{type, value};
}

template <typename Derived1, typename Derived2, typename Derived3>
bool IsA_For_Values(const The_t<Derived1, Derived2>& x, const Pie<Derived3>& type) {
    // FIXME need to synth type of x.value_ and check it against x.type_
    return AreTheSameType(x.type_, type.derived());
}

struct Var_t : Pie<Var_t> {
    Var_t() : id_{next_id_++} {}

    int id_;
    static int next_id_;
};

int Var_t::next_id_{0};

std::ostream& operator<<(std::ostream& s, const Var_t x) { return s << 'x' << x.id_; }

Var_t var() {
    return Var_t{};
}

template<typename Derived1, typename Derived2>
auto ComputeValue(const The_t<Derived1, Derived2>& x) { return x; }

Var_t ComputeValue(const Var_t var) { return var; }
