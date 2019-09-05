#pragma once

#include "pie_base.h"
#include <ostream>

template <typename Derived1, typename Derived2>
struct The_t : Pie<The_t<Derived1, Derived2>> {
    The_t(const Pie<Derived1>& type, const Pie<Derived2>& value) : type_(type), value_(value) {}

    Derived1 type_;
    Derived2 value_;
};

template <typename Derived1, typename Derived2>
std::wostream& operator<<(std::wostream& s, const The_t<Derived1, Derived2>& x) {
    return s << "(the " << x.type_ << ' ' << x.value_ << ')';
}

template <typename Derived1, typename Derived2>
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

std::wostream& operator<<(std::wostream& s, const Var_t x) { return s << 'x' << x.id_; }

Var_t var() { return Var_t{}; }

template <typename Derived1, typename Derived2, typename Derived3, typename Derived4>
bool IsTheSameAs_For_Values(const Pie<Derived1>& type,
                            const The_t<Derived2, Derived3>& lhs,
                            const Pie<Derived4>& rhs) {
    return AreTheSameType(type.derived(), lhs.type_) && IsTheSameAs(type.derived(), lhs.value_, rhs.derived());
}

template <typename T>
struct is_a_the {
    static constexpr bool value = false;
};

template <typename Derived1, typename Derived2>
struct is_a_the<The_t<Derived1, Derived2>> {
    static constexpr bool value = true;
};

template <typename T>
inline constexpr bool is_a_the_v = is_a_the<T>::value;

template <typename Derived1, typename Derived2, typename Derived3, typename Derived4, typename = std::enable_if_t<!is_a_the_v<Derived2>>>
bool IsTheSameAs_For_Values(const Pie<Derived1>& type,
                            const Pie<Derived2>& lhs,
                            const The_t<Derived3, Derived4>& rhs) {
    return AreTheSameType(type.derived(), rhs.type_) && IsTheSameAs(type.derived(), lhs.derived(), rhs.value_);
}

// FIXME check type (not because the vars might not be the same, but because they might not match the type ...)
template <typename Derived>
bool IsTheSameAs_For_Values(const Pie<Derived>&, const Var_t lhs, const Var_t rhs) {
    return lhs.id_ == rhs.id_;
}

// FIXME should be x.value_, but currently we require the type later ...
template <typename Derived1, typename Derived2>
auto ComputeValue(const The_t<Derived1, Derived2>& x) {
    return x;
}

Var_t ComputeValue(const Var_t var) { return var; }
