#pragma once

#include "pie_base.h"
#include <ostream>

template <typename Derived1, typename Derived2>
struct The_t : Pie<The_t<Derived1, Derived2>> {
    constexpr The_t(const Pie<Derived1>& type, const Pie<Derived2>& value) : type_(type), value_(value) {}

    Derived1 type_;
    Derived2 value_;
};

template <typename Derived1, typename Derived2, typename Derived3, typename Derived4>
constexpr bool equal(const The_t<Derived1, Derived2>& lhs, const The_t<Derived3, Derived4>& rhs, int& next_index) {
    return equal(lhs.value_, rhs.value_, next_index);
}

template <typename Derived1, typename Derived2>
void print(std::ostream& s, const The_t<Derived1, Derived2>& x, int& next_index) {
    s << "(the " << InContext(x.type_, next_index) << ' ' << InContext(x.value_, next_index) << ')';
}

template <typename Derived1, typename Derived2>
constexpr The_t<Derived1, Derived2> the(const Pie<Derived1>& type, const Pie<Derived2>& value) {
    return The_t<Derived1, Derived2>{type, value};
}

template <typename Type, typename Expr>
struct step_result<The_t<Type, Expr>> {
    using type = Expr;
};

template <typename Type, typename Expr>
constexpr step_result_t<The_t<Type, Expr>> Step(const The_t<Type, Expr>& x) {
    return x.value_;
}

template <typename Type, typename Expr>
struct synth_result<The_t<Type, Expr>> {
    using type = Type;
};

template <typename Type, typename Expr>
constexpr synth_result_t<The_t<Type, Expr>> synth1(const The_t<Type, Expr>& x, int&) {
    return x.type_;
}

struct Var_t : Pie<Var_t> {
    constexpr Var_t(const int id) : id_{id} {}

    int id_;
};

constexpr bool equal(const Var_t lhs, const Var_t rhs, int&) { return lhs.id_ == rhs.id_; }

inline void print(std::ostream& s, const Var_t x, int&) { s << 'x' << x.id_; }

constexpr Var_t var(const int id) { return Var_t{id}; }

template <>
struct is_neutral<Var_t> : std::true_type {};

template <typename Type>
struct TypedVar_t : Pie<TypedVar_t<Type>> {
    constexpr TypedVar_t(const Type& type, const int id) : id_{id}, type_{type} {}

    int id_;
    Type type_;
};

template <typename Type1, typename Type2>
constexpr bool equal(const TypedVar_t<Type1> lhs, const TypedVar_t<Type2> rhs, int&) {
    return lhs.id_ == rhs.id_;
}

template <typename Type>
void print(std::ostream& s, const TypedVar_t<Type> x, int& next_index) {
    s << '[' << 'x' << x.id_ << ' ' << InContext(x.type_, next_index) << ']';
}

template <typename Type>
constexpr TypedVar_t<Type> var(const Type& type, const int id) {
    return TypedVar_t<Type>{type, id};
}

template <typename Type>
struct is_neutral<TypedVar_t<Type>> : std::true_type {};

template <typename Type>
struct synth_result<TypedVar_t<Type>> {
    using type = Type;
};

template <typename Type>
constexpr synth_result_t<TypedVar_t<Type>> synth1(const TypedVar_t<Type>& x, int&) {
    return x.type_;
}
