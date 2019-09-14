#pragma once

#include "pie_base.h"
#include <ostream>

template <typename Derived1, typename Derived2>
struct The_t : Pie<The_t<Derived1, Derived2>> {
    The_t(const Pie<Derived1>& type, const Pie<Derived2>& value) : type_(type), value_(value) {}

    Derived1 type_;
    Derived2 value_;
};

template <typename Derived1, typename Derived2, typename Derived3, typename Derived4>
bool operator==(const The_t<Derived1, Derived2>& lhs, const The_t<Derived3, Derived4>& rhs) {
    return lhs.value_ == rhs.value_;
}

template <typename Derived1, typename Derived2>
std::wostream& operator<<(std::wostream& s, const The_t<Derived1, Derived2>& x) {
    return s << "(the " << x.type_ << ' ' << x.value_ << ')';
}

template <typename Derived1, typename Derived2>
The_t<Derived1, Derived2> the(const Pie<Derived1>& type, const Pie<Derived2>& value) {
    return The_t<Derived1, Derived2>{type, value};
}

template <typename Type, typename Expr>
struct step_result<The_t<Type, Expr>> {
    using type = Expr;
};

template <typename Type, typename Expr>
step_result_t<The_t<Type, Expr>> Step(const The_t<Type, Expr>& x) {
    return x.value_;
}

template <typename Type, typename Expr>
struct synth_result<The_t<Type, Expr>> {
    using type = Type;
};

template <typename Type, typename Expr>
synth_result_t<The_t<Type, Expr>> synth(const The_t<Type, Expr>& x) {
    return x.type_;
}

inline int GetNextId() {
    static int next_id{};
    return next_id++;
}

struct Var_t : Pie<Var_t> {
    Var_t() : id_{GetNextId()} {}

    int id_;
};

inline bool operator==(const Var_t lhs, const Var_t rhs) { return lhs.id_ == rhs.id_; }

inline std::wostream& operator<<(std::wostream& s, const Var_t x) { return s << 'x' << x.id_; }

inline Var_t var() { return Var_t{}; }

template <>
struct is_neutral<Var_t> : std::true_type {};

template <typename Type>
struct TypedVar_t : Pie<TypedVar_t<Type>> {
    TypedVar_t(const Type& type) : id_{GetNextId()}, type_{type} {}

    int id_;
    Type type_;
};

template <typename Type1, typename Type2>
bool operator==(const TypedVar_t<Type1> lhs, const TypedVar_t<Type2> rhs) {
    return lhs.id_ == rhs.id_;
}

template <typename Type>
std::wostream& operator<<(std::wostream& s, const TypedVar_t<Type> x) {
    return s << '[' << 'x' << x.id_ << ' ' << x.type_ << ']';
}

template <typename Type>
TypedVar_t<Type> var(const Type& type) {
    return TypedVar_t<Type>{type};
}

template <typename Type>
struct is_neutral<TypedVar_t<Type>> : std::true_type {};

template <typename Type>
struct synth_result<TypedVar_t<Type>> {
    using type = Type;
};

template <typename Type>
synth_result_t<TypedVar_t<Type>> synth(const TypedVar_t<Type>& x) {
    return x.type_;
}
