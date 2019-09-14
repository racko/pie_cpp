#pragma once

#include <cassert>
#include <iostream>
#include <tuple>
#include <type_traits>

// SORRY :(
template <typename F, typename Arg>
struct App_t;

template <typename F, typename... Args>
struct apply_result;

template <typename F, typename... Args>
using apply_result_t = typename apply_result<F, Args...>::type;

template <typename F>
struct apply_result<F> {
    using type = F;
};

template <typename F, typename Arg, typename... Args>
struct apply_result<F, Arg, Args...> {
    using type = apply_result_t<App_t<F, Arg>, Args...>;
};

template <typename Derived>
struct Pie {
    template <typename... Args>
    apply_result_t<Derived, Args...> operator()(const Pie<Args>&... args) const;

    constexpr operator Derived&() { return static_cast<Derived&>(*this); }
    constexpr operator Derived const&() const { return static_cast<const Derived&>(*this); }

    constexpr Derived& derived() { return static_cast<Derived&>(*this); }
    constexpr Derived const& derived() const { return static_cast<const Derived&>(*this); }
};

template <typename Derived1, typename Derived2>
bool operator==(const Pie<Derived1>& lhs, const Pie<Derived2>& rhs) {
    std::wcerr << "operator==(" << lhs.derived() << ", " << rhs.derived() << "): false (default)\n";
    return false;
}

template <typename T>
struct is_neutral : std::false_type {};

template <typename T>
inline constexpr bool is_neutral_v = is_neutral<T>::value;

template <typename Derived>
bool IsNeutral(const Pie<Derived>&) {
    return is_neutral_v<Derived>;
}

template <typename T>
struct is_normal : std::bool_constant<is_neutral_v<T>> {};

template <typename T>
inline constexpr bool is_normal_v = is_normal<T>::value;

template <typename Derived>
bool IsNormal(const Pie<Derived>&) {
    return is_normal_v<Derived>;
}

template <typename T>
struct is_value : std::bool_constant<is_normal_v<T>> {};

template <typename T>
inline constexpr bool is_value_v = is_value<T>::value;

template <typename Derived>
bool IsValue(const Pie<Derived>&) {
    return is_value_v<Derived>;
}

template <typename T>
struct step_result;

template <typename T>
using step_result_t = typename step_result<T>::type;

template <typename Derived>
step_result_t<Derived> Step(const Pie<Derived>&);

template <typename T>
struct compute_value_result;

template <typename T, bool IsValue>
struct compute_value_result1;

template <typename T>
struct compute_value_result1<T, true> {
    using type = T;
};

template <typename T>
struct compute_value_result1<T, false> {
    using type = typename compute_value_result<step_result_t<T>>::type;
};

template <typename T>
struct compute_value_result {
    using type = typename compute_value_result1<T, is_value_v<T>>::type;
};

template <typename T>
using compute_value_result_t = typename compute_value_result<T>::type;

template <typename Derived>
compute_value_result_t<Derived> ComputeValue(const Pie<Derived>& x, std::true_type) {
    std::wcerr << "ComputeValue(" << x.derived() << ", is_value=true) ...\n";
    return x.derived();
}

template <typename Derived>
compute_value_result_t<Derived> ComputeValue(const Pie<Derived>& x, std::false_type) {
    std::wcerr << "ComputeValue(" << x.derived() << ", is_value=false) ...\n";
    return ComputeValue(Step(x.derived()));
}

template <typename Derived>
compute_value_result_t<Derived> ComputeValue(const Pie<Derived>& x) {
    std::wcerr << "ComputeValue(" << x.derived() << ") ...\n";
    return ComputeValue(x.derived(), is_value<Derived>{});
}

template <typename T>
struct normalize_result;

template <typename T, bool IsNormal>
struct normalize_result1;

template <typename T>
struct normalize_result1<T, true> {
    using type = T;
};

template <typename T>
struct normalize_result1<T, false> {
    using type = typename normalize_result<step_result_t<T>>::type;
};

template <typename T>
struct normalize_result {
    using type = typename normalize_result1<T, is_normal_v<T>>::type;
};

template <typename T>
using normalize_result_t = typename normalize_result<T>::type;

template <typename Derived>
normalize_result_t<Derived> Normalize(const Pie<Derived>& x, std::true_type /*is_normal*/) {
    std::wcerr << "Normalize(" << x.derived() << ", is_normal=true) ...\n";
    return x.derived();
}

template <typename Derived>
normalize_result_t<Derived> Normalize(const Pie<Derived>& x, std::false_type /*is_normal*/) {
    std::wcerr << "Normalize(" << x.derived() << ", is_normal=false) ...\n";
    return Normalize(Step(x.derived()));
}

template <typename Derived>
normalize_result_t<Derived> Normalize(const Pie<Derived>& x) {
    std::wcerr << "Normalize(" << x.derived() << ") ...\n";
    return Normalize(x.derived(), is_normal<Derived>{});
}

template <typename T>
struct synth_result;

template <typename T>
using synth_result_t = typename synth_result<T>::type;

template <typename Derived>
synth_result_t<Derived> synth(const Pie<Derived>& x);

template <typename Derived1, typename Derived2>
bool IsA(const Pie<Derived1>& value, const Pie<Derived2>& type) {
    std::wcerr << "IsA(" << value.derived() << ", " << type.derived() << ") [default] ... \n";
    return AreTheSameType(type.derived(), synth(value.derived()));
}

template <typename Derived1, typename Derived2>
bool IsANormal(const Pie<Derived1>& value, const Pie<Derived2>& type) {
    return IsA(value.derived(), type.derived()) && is_normal_v<Derived1>;
}

template <typename Derived1, typename Derived2, typename Derived3>
bool IsTheSameAs(const Pie<Derived1>& type, const Pie<Derived2>& lhs, const Pie<Derived3>& rhs) {
    std::wcerr << "IsTheSameAs(" << type.derived() << ", " << lhs.derived() << ", " << rhs.derived() << ") ... \n";
    const auto is_type_valid = IsAType(type.derived());
    std::wcerr << "type is valid: " << is_type_valid << '\n';
    if (!is_type_valid) {
        return false;
    }
    const auto lhs_norm = Normalize(lhs.derived());
    const auto rhs_norm = Normalize(rhs.derived());
    std::wcerr << "lhs normalform: " << lhs_norm << '\n';
    std::wcerr << "rhs normalform: " << rhs_norm << '\n';
    const auto normalforms_identical = lhs_norm == rhs_norm;
    std::wcerr << "normalforms are identical: " << normalforms_identical << '\n';
    return normalforms_identical;
}

template <typename Derived1, typename Derived2>
bool IsAValue(const Pie<Derived1>& type, const Pie<Derived2>& expr) {
    return IsA(expr.derived(), type.derived()) && is_value_v<Derived2>;
}

template <typename Derived1, typename Derived2>
bool AreTheSameType_For_Values(const Pie<Derived1>&, const Pie<Derived2>&) {
    return false;
}

template <typename Derived1, typename Derived2>
bool AreTheSameType(const Pie<Derived1>& lhs, const Pie<Derived2>& rhs) {
    std::wcerr << "AreTheSameType(" << lhs.derived() << ", " << rhs.derived() << ") ... \n";
    const auto lhs_norm = Normalize(lhs.derived());
    const auto rhs_norm = Normalize(rhs.derived());
    std::wcerr << "lhs normalform: " << lhs_norm << '\n';
    std::wcerr << "rhs normalform: " << rhs_norm << '\n';
    const auto normalforms_identical = lhs_norm == rhs_norm;
    std::wcerr << "normalforms are identical: " << normalforms_identical << '\n';
    return normalforms_identical;
    // return AreTheSameType_For_Values(ComputeValue(lhs.derived()), ComputeValue(rhs.derived()));
}

template <typename Derived1, typename Derived2>
bool IsNormalFormOfType(const Pie<Derived1>& nf, const Pie<Derived2>& type) {
    std::wcerr << "IsNormalFormOfType(" << nf.derived() << ", " << type.derived() << ") ...\n";
    const auto actual = Normalize(type.derived());
    std::wcerr << "rhs normal form: " << actual << '\n';
    const auto identical = actual == nf.derived();
    std::wcerr << "rhs normal form == lhs: " << identical << '\n';
    return identical;
}

template <typename Derived1, typename Derived2, typename Derived3>
bool IsNormalFormOf(const Pie<Derived1>& type, const Pie<Derived2>& lhs, const Pie<Derived3>& rhs) {
    std::wcerr << "IsNormalFormOf(" << type.derived() << ", " << lhs.derived() << ", " << rhs.derived() << ") ...\n";
    const auto actual = Normalize(rhs.derived());
    std::wcerr << "rhs normal form: " << actual << '\n';
    const auto identical = actual == lhs.derived();
    std::wcerr << "rhs normal form == lhs: " << identical << '\n';
    return identical;
}

template <typename Derived1, typename Derived2, typename Derived3>
bool IsTheValueOf(const Pie<Derived1>& type, const Pie<Derived2>& lhs, const Pie<Derived3>& rhs) {
    std::wcerr << "IsTheValueOf(" << type.derived() << ", " << lhs.derived() << ", " << rhs.derived() << ") ... \n";
    const auto rhs_value = ComputeValue(rhs.derived());
    std::wcerr << "rhs value: " << rhs_value << '\n';
    const auto identical = rhs_value == lhs.derived();
    std::wcerr << "rhs value == lhs: " << identical << '\n';
    return identical;
}

template <typename Type, typename Expr>
struct Definition : Pie<Definition<Type, Expr>> {
    constexpr Definition(const wchar_t* name, const Type& type, const Expr& expr)
        : name_{name}, type_{type}, expr_{expr} {}

    const wchar_t* name_;
    Type type_;
    Expr expr_;
};

template <typename Type1, typename Expr1, typename Type2, typename Expr2>
bool operator==(const Definition<Type1, Expr1>& lhs, const Definition<Type2, Expr2>& rhs) {
    return lhs.name_ == rhs.name_;
}

template <typename Type, typename Expr, typename Derived>
bool operator==(const Definition<Type, Expr>& lhs, const Pie<Derived>& rhs) {
    return lhs.expr_ == rhs.derived();
}

template <typename Derived, typename Type, typename Expr>
bool operator==(const Pie<Derived>& lhs, const Definition<Type, Expr>& rhs) {
    return lhs.derived() == rhs.expr_;
}

template <typename Type, typename Expr>
std::wostream& operator<<(std::wostream& s, const Definition<Type, Expr>& def) {
    return s << def.name_;
}

template <typename Type, typename Expr>
constexpr Definition<Type, Expr> define(const wchar_t* name, const Type& type, const Expr& expr) {
    std::wcout << "checking define " << name << " ...\n";
    assert(IsA(expr.derived(), type.derived()));
    std::wcout << "checking define " << name << " done\n";
    return Definition(name, type, expr);
}

template <typename Type, typename Expr>
struct step_result<Definition<Type, Expr>> {
    using type = Expr;
};

template <typename Type, typename Expr>
step_result_t<Definition<Type, Expr>> Step(const Definition<Type, Expr>& x) {
    return x.expr_;
}

template <typename Type, typename Expr>
struct synth_result<Definition<Type, Expr>> {
    using type = Type;
};

template <typename Type, typename Expr>
synth_result_t<Definition<Type, Expr>> synth(const Definition<Type, Expr>& x) {
    return x.type_;
}

struct U_t : Pie<U_t> {};

constexpr bool operator==(U_t, U_t) { return true; }

inline std::wostream& operator<<(std::wostream& s, U_t) { return s << "U"; }

inline constexpr U_t U;

template <>
struct is_normal<U_t> : std::true_type {};

template <>
struct is_value<U_t> : std::true_type {};

constexpr bool IsAType(U_t) { return true; }

template <typename Derived>
bool IsA(U_t, const Pie<Derived>& type) {
    std::wcerr << "IsA(U, " << type.derived() << "): false (U does not have a type) \n";
    return false;
}

template <typename Derived>
bool IsAType(const Pie<Derived>& type) {
    return IsA(type.derived(), U);
}
